/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPClient.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 22:15:19 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 22:15:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <buffer/impl/BaseBuffer.hpp>
#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <buffer/impl/SocketBuffer.hpp>
#include <config/block/LocationBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/enums/HTTPVersion.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <http/parser/exception/status/HTTPRequestHeaderTooBigException.hpp>
#include <http/parser/exception/status/HTTPRequestPayloadTooLargeException.hpp>
#include <http/parser/exception/status/HTTPRequestURLTooLongException.hpp>
#include <http/response/body/IResponseBody.hpp>
#include <http/task/HTTPTask.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <sys/types.h>
#include <unit/DataSize.hpp>
#include <util/Enum.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <util/System.hpp>
#include <util/Time.hpp>
#include <util/URL.hpp>
#include <iostream>
#include <typeinfo>

Logger &HTTPClient::LOG = LoggerFactory::get("HTTP Client");

HTTPClient::HTTPClient(Socket &socket, InetSocketAddress socketAddress, HTTPServer &server) : m_socket(socket),
																							  m_socketAddress(socketAddress),
																							  m_in(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
																							  m_out(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
																							  m_server(server),
																							  m_parser(*this),
																							  m_body(),
																							  m_state(S_NOT_STARTED),
																							  m_lastAction(),
																							  m_request(),
																							  m_response(),
																							  m_filterChain(*this, m_request, m_response),
																							  m_task(),
																							  m_keepAlive(true)
{
	NIOSelector::instance().add(m_socket, *this, NIOSelector::READ);
	updateLastAction();
}

HTTPClient::~HTTPClient(void)
{
	LOG.trace() << "Deleting: " << m_socket.raw() << std::endl;

	delete &m_in;
	delete &m_out;

	log();

	NIOSelector::instance().remove(m_socket);
	DeleteHelper::pointer(m_task);

	delete &m_socket;

	httpServer().untrack(*this);
}

void HTTPClient::reset()
{
	LOG.trace() << "Resetting: " << m_socket.raw() << std::endl;

	m_parser.reset();
	m_body.clear();
	m_state = S_NOT_STARTED;
	m_request = HTTPRequest();
	m_response = HTTPResponse();
	DeleteHelper::pointer(m_task);
	m_keepAlive = true;

	updateLastAction();

	NIOSelector::instance().remove(m_socket);
	NIOSelector::instance().add(m_socket, *this, NIOSelector::READ);

	if (!m_in.empty())
	{
		m_state = S_HEADER;
		readHead();
	}
}

void HTTPClient::log()
{
	if (!LOG.isInfoEnabled())
		return;

	std::ostream &out = LOG.info()
						/**/
						<< '[' << Time::now().cformat(HTTPCLIENT_LOG_TIME_FORMAT) << "] "
						/**/
						<< m_socketAddress.hostAddress()
						/**/
						<< " - ";

	if (m_request.method().present())
	{
		out << m_request.method().get()->name()
			/**/
			<< " "
			/**/
			<< m_request.url().path()
			/**/
			<< " :: ";

		if (m_response.status().present())
			out << m_response.status().get()->code();
		else
			out << "<no response>";
	}
	else
		out << "<no request>";

	out << std::endl;
}

void HTTPClient::updateLastAction()
{
	long time = System::currentTimeSeconds();

	if (time)
		m_lastAction = time;
}

bool HTTPClient::writable(FileDescriptor &fd)
{
	(void)fd;

	if (!m_response.ended())
		return (false);

	bool finished = m_response.store(m_out);

	ssize_t r = 0;
	if ((r = m_out.send()) > 0)
		updateLastAction();

	if (r == -1)
		delete this;
	else if (finished && m_out.empty())
	{
		log();

		if (m_keepAlive)
			reset();
		else
			delete this;
	}

	return (false);
}

bool HTTPClient::readable(FileDescriptor &fd)
{
	(void)fd;

	if (m_in.size())
	{
		if (m_in.recv(0, 1) <= 0)
		{
			delete this;
			return (true);
		}

		return (doRead());
	}

	if (m_in.recv() <= 0)
	{
		delete this;
		return (true);
	}

	return (doRead());
}

bool HTTPClient::doRead(void)
{
	updateLastAction();

	switch (m_state)
	{
	case S_NOT_STARTED:
		m_state = S_HEADER;

	case S_HEADER:
		return (readHead());

	case S_BODY:
		return (readBody());

	case S_END:
		return (true);
	}

	return (false);
}

bool HTTPClient::readHead(void)
{
	char c;

	while (m_in.next(c))
	{
		bool catched = true;

		try
		{
			m_parser.consume(c);
			//	std::cout << c;

			if (m_parser.state() == HTTPRequestParser::S_END)
			{
				m_request = HTTPRequest(m_parser.version(), m_parser.url(), m_parser.headerFields());
				m_filterChain.doChainingOf(FilterChain::S_BEFORE);

				if (m_response.ended())
				{
					m_filterChain.doChainingOf(FilterChain::S_AFTER);
					m_state = S_END;

					if (m_request.method().get()->hasBody())
					{
						std::cout << "INSIDE\n";
						long long maxBodySize = isMaxBodySize(m_request.serverBlock(), m_request.locationBlock());

						if (maxBodySize != -1)
							m_parser.maxBodySize(maxBodySize);

						m_parser.state() = HTTPRequestParser::S_BODY;
						m_state = S_BODY;
						m_parser.consume(0);

						if (m_parser.state() != HTTPRequestParser::S_END) /* No body */
						{
							std::cout << "BODY\n";
							// if (!(m_in.storage().empty()))
							// {
							// 	std::cout << "not empty1\n";
							// 	m_in.skip(m_in.storage().size());
							// }
							std::cout << "storage : " << m_in.storage().size() << "END"<< std::endl;
								readBody();
						}

						return (true);
					}
				}
				//	std::cout << "HEREEEEE\n";
				if (m_request.method().get()->hasBody())
				{
					long long maxBodySize = isMaxBodySize(m_request.serverBlock(), m_request.locationBlock());

					if (maxBodySize != -1)
						m_parser.maxBodySize(maxBodySize);

					m_parser.state() = HTTPRequestParser::S_BODY;
					m_state = S_BODY;
					m_parser.consume(0);

					if (m_parser.state() != HTTPRequestParser::S_END) /* No body */
					{
						// if (!(m_in.storage().empty()))
						// {
						// 	std::cout << "not empty1\n";
						// 	m_in.skip(m_in.storage().size());
						// }
						return (readBody());
					}
				}
				else
				{
					NIOSelector::instance().update(m_socket, NIOSelector::NONE);
					// if (!(m_in.storage().empty()))
					// {
					// 	std::cout << "not empty2\n";
					// 		m_in.skip(m_in.storage().size());
					// }
					m_filterChain.doChainingOf(FilterChain::S_BETWEEN);
					m_state = S_END;
				}
			}

			catched = false;
		}
		catch (HTTPRequestHeaderTooBigException &exception)
		{
			m_response.status(*HTTPStatus::REQUEST_ENTITY_TOO_LARGE);
		}
		catch (HTTPRequestURLTooLongException &exception)
		{
			m_response.status(*HTTPStatus::URI_TOO_LONG);
		}
		catch (Exception &exception)
		{
			LOG.debug() << "Failed to process header: " << exception.message() << std::endl;

			m_response.status(*HTTPStatus::BAD_REQUEST);
		}

		if (catched)
		{
			LOG.debug() << "doing after" << std::endl;
			NIOSelector::instance().update(m_socket, NIOSelector::NONE);
			m_filterChain.doChainingOf(FilterChain::S_AFTER);
			m_state = S_END;

			break;
		}
	}

	return (false);
}

bool HTTPClient::readBody(void)
{
	if (!m_in.empty())
	{
		try
		{
			m_parser.consume(0);

			if (m_parser.state() == HTTPRequestParser::S_END)
			{
				NIOSelector::instance().update(m_socket, NIOSelector::NONE);
				m_filterChain.doChainingOf(FilterChain::S_BETWEEN);
				m_state = S_END;
			}
		}
		catch (HTTPRequestPayloadTooLargeException &exception)
		{
			m_response.status(*HTTPStatus::PAYLOAD_TOO_LARGE);
			m_filterChain.doChainingOf(FilterChain::S_AFTER);
			m_state = S_END;
		}
		catch (Exception &exception)
		{
			m_response.status(*HTTPStatus::UNPROCESSABLE_ENTITY); /* TODO Need more specific message based on the problem. */
			m_filterChain.doChainingOf(FilterChain::S_AFTER);
			m_state = S_END;
		}
	}

	return (false);
}

std::string &
HTTPClient::body()
{
	return (m_body);
}

HTTPTask *
HTTPClient::task()
{
	return (m_task);
}

void HTTPClient::task(HTTPTask &task, bool removePrevious)
{
	if (removePrevious && m_task)
		delete m_task;

	m_task = &task;
}

long long
HTTPClient::isMaxBodySize(const Optional<const ServerBlock *> &serverBlock, const Optional<const LocationBlock *> &locationBlock)
{
	if (locationBlock.present() && (*locationBlock.get()).hasMaxBodySize())
		return ((*locationBlock.get()).maxBodySize().get().toBytes());

	if (serverBlock.present() && (*serverBlock.get()).hasMaxBodySize())
		return ((*serverBlock.get()).maxBodySize().get().toBytes());

	return (-1);
}

void HTTPClient::keepAlive(bool keepAlive)
{
	m_keepAlive = keepAlive;
}
