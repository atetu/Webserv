/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 16:38:46 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/09 15:52:11 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTP.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <sstream>

GenericHTTPResponse::IBody::~IBody()
{
}

GenericHTTPResponse::FileBody::FileBody(FileDescriptorBuffer &fileBuffer) :
		m_fileDescriptorBuffer(fileBuffer)
{
}

FileDescriptorBuffer&
GenericHTTPResponse::FileBody::fileDescriptorBuffer()
{
	return (m_fileDescriptorBuffer);
}

GenericHTTPResponse::FileBody::~FileBody()
{
}

bool
GenericHTTPResponse::FileBody::write(SocketBuffer &socketBuffer)
{
	socketBuffer.store(m_fileDescriptorBuffer);

	return (isDone());
}

bool
GenericHTTPResponse::FileBody::isDone(void)
{
	return (m_fileDescriptorBuffer.hasReadEverything() && m_fileDescriptorBuffer.size() == 0);
}

GenericHTTPResponse::StringBody::StringBody(std::string string) :
		m_string(string),
		m_sent(false)
{
}

GenericHTTPResponse::StringBody::~StringBody()
{
}

bool
GenericHTTPResponse::StringBody::write(SocketBuffer &socketBuffer)
{
	if (!m_sent)
	{
		socketBuffer.store(m_string);
		m_sent = true;
	}

	return (true);
}

bool
GenericHTTPResponse::StringBody::isDone(void)
{
	return (m_sent);
}

GenericHTTPResponse::GenericHTTPResponse(const HTTPStatusLine statusLine, const HTTPHeaderFields &headers, IBody *body) :
		m_statusLine(statusLine),
		m_headers(headers),
		m_body(body),
		m_state(NONE),
		m_state_index(-1)
{
}

GenericHTTPResponse::~GenericHTTPResponse()
{
	if (m_body)
		delete m_body;
}

HTTPStatusLine
GenericHTTPResponse::statusLine() const
{
	return (m_statusLine);
}

bool
GenericHTTPResponse::write(SocketBuffer &socketBuffer)
{
	if (m_state == NONE)
		m_state = STATUS_LINE;

	switch (m_state)
	{
		case STATUS_LINE:
			socketBuffer.store(m_statusLine.format());
			socketBuffer.store(HTTP::CRLF);

			m_state = HEADERS;

			break;

		case HEADERS:
			socketBuffer.store(m_headers.format());
			socketBuffer.store(HTTP::CRLF);

			if (m_body)
				m_state = BODY;
			else
				m_state = FLUSHING;

			break;

		case BODY:
			if (m_body && m_body->write(socketBuffer))
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
GenericHTTPResponse::readFileDescriptors(fdb_vector &out)
{
	FileBody *fileBody = dynamic_cast<FileBody*>(m_body);
	if (fileBody)
		out.push_back(&fileBody->fileDescriptorBuffer());
}

void
GenericHTTPResponse::writeFileDescriptors(fdb_vector &out)
{
	(void)out;
}

GenericHTTPResponse::State
GenericHTTPResponse::state() const
{
	return (m_state);
}

GenericHTTPResponse*
GenericHTTPResponse::status(HTTPStatus &status)
{
	return (new GenericHTTPResponse(HTTPStatusLine(HTTPVersion::HTTP_1_1, status), HTTPHeaderFields(), NULL));
}
