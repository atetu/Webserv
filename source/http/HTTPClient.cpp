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

#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <buffer/impl/SocketBuffer.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/enums/HTTPVersion.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <http/parser/exception/status/HTTPRequestHeaderTooBigException.hpp>
#include <http/parser/exception/status/HTTPRequestURLTooLongException.hpp>
#include <http/task/HTTPTask.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <util/Enum.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <util/System.hpp>
#include <util/Time.hpp>
#include <util/URL.hpp>
#include <iostream>

Logger &HTTPClient::LOG = LoggerFactory::get("HTTP Client");

HTTPClient::HTTPClient(Socket &socket, InetSocketAddress socketAddress, HTTPServer &server) :
		m_socket(socket),
		m_socketAddress(socketAddress),
		m_in(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
		m_out(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
		m_server(server),
		m_parser(m_body),
		m_body(),
		m_state(S_NOT_STARTED),
		m_lastAction(),
		m_request(),
		m_response(),
		m_filterChain(*this, m_request, m_response),
		m_task(),
		m_ending(false)
{
	updateLastAction();
}

HTTPClient::~HTTPClient(void)
{
	delete &m_in;
	delete &m_out;

	log();

	DeleteHelper::pointer(m_task);

	NIOSelector::instance().remove(m_socket);
	delete &m_socket;

	httpServer().untrack(*this);
}

void
HTTPClient::log()
{
	if (!LOG.isInfoEnabled())
		return;

	std::ostream &out = LOG.info()
	/**/<< '[' << Time::now().cformat(HTTPCLIENT_LOG_TIME_FORMAT) << "] "
	/**/<< m_socketAddress.hostAddress()
	/**/<< " - ";

	if (m_request.method().present())
	{
		out << m_request.method().get()->name()
		/**/<< " "
		/**/<< m_request.url().path()
		/**/<< " :: ";

		if (m_response.status().present())
			out << m_response.status().get()->code();
		else
			out << "<no response>";
	}
	else
		out << "<no request>";

	out << std::endl;
}

void
HTTPClient::updateLastAction()
{
	long time = System::currentTimeSeconds();

	if (time)
		m_lastAction = time;
}

bool
HTTPClient::writable(FileDescriptor &fd)
{
	(void)fd;

	bool finished = false;
	if (m_out.capacity() && m_response.store(m_out))
		finished = true;

	ssize_t r = 0;
	if ((r = m_out.send()) > 0)
		updateLastAction();

	if (r == -1)
		delete this;
	else if (finished && m_out.empty())
	{
		m_ending = true;
		m_server.ending(*this);
	}

	return (finished);
}

bool
HTTPClient::readable(FileDescriptor &fd)
{
	(void)fd;

	ssize_t r = 1;
	bool cond = m_in.size() != 0 || (r = m_in.recv()) > 0;

	if (r == -1 || (m_ending && r == 0))
		delete this;
	else
	{
		if (cond)
		{
			switch (m_state)
			{
				case S_NOT_STARTED:
					m_state = S_HEADER;

					/* Falling */

				case S_HEADER:
					return (readableHead());

				case S_BODY:
					return (readableBody());

				case S_END:
					return (true);
			}
		}
	}

	return (false);
}

bool
HTTPClient::readableHead(void)
{
	char c;

	while (m_in.next(c))
	{
		bool catched = true;
		std::cout << c;
		try
		{
			m_parser.consume(c);

			if (m_parser.state() == HTTPRequestParser::S_END)
			{
				m_request = HTTPRequest(m_parser.version(), m_parser.url(), m_parser.headerFields());
				m_filterChain.doChainingOf(FilterChain::S_BEFORE);

				if (m_response.status().present())
				{
					m_filterChain.doChainingOf(FilterChain::S_AFTER);
					NIOSelector::instance().update(m_socket, NIOSelector::WRITE);
					m_state = S_END;
				}
				else
				{
					if (m_request.method().present() && m_request.method().get()->hasBody())
					{
						std::cout << "body first\n";
						// if (m_request.serverBlock().present() && m_request.serverBlock().get()->maxBodySize().present())
						// 	m_parser.maxBodySize(m_request.serverBlock().get()->maxBodySize().get().toBytes());
						long long maxBodySize = isMaxBodySize(m_request.serverBlock(), m_request.locationBlock());
					//	std::cout << "max : " << maxBodySize << std::endl;
						if (maxBodySize != -1)
						{
							m_parser.maxBodySize(maxBodySize);
					//		std::cout << "max : " << maxBodySize << std::endl;
						}
						m_parser.state() = HTTPRequestParser::S_BODY;
						m_state = S_BODY;
						m_parser.consume(0);

						if (m_parser.state() == HTTPRequestParser::S_END) /* No body */
						{
							NIOSelector::instance().update(m_socket, NIOSelector::NONE);
							m_filterChain.doChainingOf(FilterChain::S_BETWEEN);
							m_state = S_END;

							break;
						}

						return (readableBody());
					}
					else
					{
						NIOSelector::instance().update(m_socket, NIOSelector::NONE);
						m_filterChain.doChainingOf(FilterChain::S_BETWEEN);
						m_state = S_END;
					}
				}
				break;
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
			LOG.debug() << exception.message() << std::endl;

			m_response.status(*HTTPStatus::BAD_REQUEST);
		}

		if (catched)
		{
			m_filterChain.doChainingOf(FilterChain::S_AFTER);
			NIOSelector::instance().update(m_socket, NIOSelector::WRITE);
			m_state = S_END;
		}
	}

	return (false);
}

bool
HTTPClient::readableBody(void)
{
	char c;

	while (m_in.next(c))
	{
		try
		{
			m_parser.consume(c);

			if (m_parser.state() == HTTPRequestParser::S_END)
			{
				NIOSelector::instance().update(m_socket, NIOSelector::NONE);
				m_filterChain.doChainingOf(FilterChain::S_BETWEEN);
				m_state = S_END;

				break;
			}
		}
		catch (Exception &exception)
		{
			std::cout <<exception.message() << std::endl;
			LOG.debug() << exception.message() << std::endl;
			if (exception.message() == "Too large payload")
				m_response.status(*HTTPStatus::PAYLOAD_TOO_LARGE); 
			else
				m_response.status(*HTTPStatus::UNPROCESSABLE_ENTITY); /* TODO Need more specific message based on the problem. */
			m_filterChain.doChainingOf(FilterChain::S_AFTER);
			NIOSelector::instance().update(m_socket, NIOSelector::WRITE);
			m_state = S_END;
		}
	} //TODO Fix when there is no body
	return (false);
}

const std::string&
HTTPClient::body()
{
	return (m_body);
}

HTTPTask*
HTTPClient::task()
{
	return (m_task);
}

void
HTTPClient::task(HTTPTask &task, bool removePrevious)
{
	if (removePrevious && m_task)
		delete m_task;

	m_task = &task;
}


long long
HTTPClient::isMaxBodySize(const Optional<const ServerBlock*> &serverBlock, const Optional<const LocationBlock*> &locationBlock)
{
	if (locationBlock.present() && (*locationBlock.get()).hasMaxBodySize())
		return ((*locationBlock.get()).maxBodySize().get().toBytes());
	
	if (serverBlock.present() && (*serverBlock.get()).hasMaxBodySize())
		return ((*serverBlock.get()).maxBodySize().get().toBytes());

	return (-1);
}