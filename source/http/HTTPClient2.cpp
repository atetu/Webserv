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
#include <exception/Exception.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/HTTPClient.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <util/System.hpp>
#include <iostream>
#include <string>

Logger &HTTPClient::LOG = LoggerFactory::get("HTTP Client");

HTTPClient::HTTPClient(Socket &socket, InetSocketAddress socketAddress, const HTTPServer &server) :
		m_state(S_NOT_STARTED),
		m_socket(socket),
		m_socketAddress(socketAddress),
		m_in(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
		m_out(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
		m_server(server),
		m_parser(),
		m_lastAction(),
		m_request(),
		m_response(),
		m_filterChain(*this, m_request, m_response)
{
	updateLastAction();
}

HTTPClient::~HTTPClient(void)
{
	delete &m_in;
	delete &m_out;

	NIOSelector::instance().remove(m_socket);
	delete &m_socket;
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

	if (m_out.send() > 0)
		updateLastAction();

	if (finished && m_out.empty())
		delete this;

	return (finished);
}

bool
HTTPClient::readable(FileDescriptor &fd)
{
	(void)fd;

	if (m_in.size() != 0 || m_in.recv() > 0)
	{
		switch(m_state)
		{

			case S_NOT_STARTED:
			case S_HEADER:
			{
				if (m_response.status().absent())
				{
					
						char c;

						while (m_in.next(c))
						{
			//				std::cout << c << std::flush;

							try
							{
								m_parser.consume(c);
				
								if (m_parser.state() == HTTPRequestParser::S_END)
								{
									if (m_request.method().present() && m_request.method().get()->hasBody())
										m_state = S_BODY;
									else
										m_state = S_END;

									break;
								}					
									
							}
							catch (Exception &exception)
							{
								LOG.debug() << exception.message() << std::endl;

								m_response.status(*HTTPStatus::BAD_REQUEST);
							}
						}
						
					}
					break;	
			}
			case S_BODY:
			{
					if (m_response.status().absent())
					{
						try
						{
							if (m_parser.state() == HTTPRequestParser::S_END)
							{
								m_request = HTTPRequest(m_parser.version(), m_parser.url(), m_parser.headerFields());
								m_filterChain.doChaining();

								if (m_response.status().absent() && m_request.method().present() && m_request.method().get()->hasBody())
									m_parser.state() = HTTPRequestParser::S_BODY;

								if (!m_response.body())
								{
									m_filterChain.doChaining();
									NIOSelector::instance().update(m_socket, NIOSelector::WRITE);
								}
							}
						}
						catch (Exception &exception)
						{
							m_response.status(*HTTPStatus::INTERNAL_SERVER_ERROR);
						}
					}
					break;
			}
			
			}
	}

	return (false);
}
