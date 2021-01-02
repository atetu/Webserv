/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHTTPResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 21:09:07 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/20 21:09:07 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/Exception.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTP.hpp>
#include <http/response/impl/cgi/CGIHTTPResponse.hpp>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <util/Enum.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/Number.hpp>
#include <util/Optional.hpp>
#include <iostream>
#include <string>
#include <vector>

CGIHTTPResponse::CGIHTTPResponse(const HTTPStatusLine &statusLine, CommonGatewayInterface &cgi) :
		m_statusLine(statusLine),
		m_cgi(cgi),
		m_inBuffer(FileDescriptorBuffer::from(cgi.in(), FileDescriptorBuffer::NOTHING)),
		m_outBuffer(FileDescriptorBuffer::from(cgi.out(), FileDescriptorBuffer::NOTHING)),
		m_state(NONE),
		m_headerFieldsParser(),
		m_peekIndex()
{
}

CGIHTTPResponse::~CGIHTTPResponse()
{
	m_cgi.exit();

	DeleteHelper::pointer<FileDescriptorBuffer>(m_inBuffer);
	DeleteHelper::pointer<FileDescriptorBuffer>(m_outBuffer);

	delete &m_cgi;
}

bool
CGIHTTPResponse::write(SocketBuffer &socketBuffer)
{
//	if (m_state == NONE)
//		m_state = STATUS_LINE;

//	static int running = -1;
//	if (m_cgi.running() != running)
//	{
//		running = m_cgi.running();
//		std::cout << "running: " << m_cgi.running() << ", " << m_outBuffer->storage() << std::endl;
//	}

	switch (m_state)
	{
		case NONE:
		{
			try
			{
				char c;
				while (m_outBuffer->peek(c, m_peekIndex))
				{
					m_peekIndex++;

					m_headerFieldsParser.consume(c);
					if (m_headerFieldsParser.state() == HTTPHeaderFieldsParser::S_END)
					{

						const HTTPHeaderFields &headerFields = m_headerFieldsParser.headerFields();

						Optional<std::string> statusOptional = headerFields.get("Status");
						if (statusOptional.present())
						{
							try
							{
								std::string codePart = statusOptional.get().substr(0, statusOptional.get().find(' '));

								int code = Number::parse<int>(codePart);
								const HTTPStatus *newStatus = HTTPStatus::findByCode(code);

								if (newStatus)
									m_statusLine = HTTPStatusLine(m_statusLine.version(), *newStatus);
							}
							catch (Exception &exception)
							{
								LoggerFactory::get("CGI Response").error() << exception.message() << std::endl;
							}
						}

						m_state = STATUS_LINE;
						break;
					}
				}
			}
			catch (Exception &exception)
			{
				std::cout << exception.message() << std::endl;
				m_state = STATUS_LINE;
			}

			break;
		}

		case STATUS_LINE:
			socketBuffer.store(m_statusLine.format());
			socketBuffer.store(HTTP::CRLF);

			m_state = BODY;

			break;

		case BODY:
			socketBuffer.store(*m_outBuffer);

			if (!m_cgi.running() || m_outBuffer->hasReadEverything())
				m_state = FLUSHING;

			break;

		case FLUSHING:
			if (socketBuffer.size() == 0)
				m_state = FINISHED;

			break;

		default:
			break;
	}

	return (m_state == FINISHED);
}

void
CGIHTTPResponse::readFileDescriptors(fdb_vector &out)
{
	out.push_back(m_outBuffer);
}

void
CGIHTTPResponse::writeFileDescriptors(fdb_vector &out)
{
	out.push_back(m_inBuffer);
}

HTTPStatusLine
CGIHTTPResponse::statusLine() const
{
	return (m_statusLine);
}

CGIHTTPResponse::State
CGIHTTPResponse::state() const
{
	return (m_state);
}
