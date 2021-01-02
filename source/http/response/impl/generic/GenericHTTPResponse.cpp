/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GenericHTTPResponse.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 16:38:46 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/27 11:09:27 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/enums/HTTPStatus.hpp>
#include <http/HTTP.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <iostream>
#include <vector>

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
	delete &m_fileDescriptorBuffer;
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

GenericHTTPResponse::FileAndStringBody::FileAndStringBody(FileDescriptorBuffer &fileBuffer, std::string string) :
		m_fileDescriptorBuffer(fileBuffer),
		m_fdSent(false),
		m_string(string),
		m_strSent(false)
{
	(void)m_fdSent; // TODO Field not used
}

GenericHTTPResponse::FileAndStringBody::~FileAndStringBody()
{
}

FileDescriptorBuffer&
GenericHTTPResponse::FileAndStringBody::fileDescriptorBuffer()
{
	return (m_fileDescriptorBuffer);
}

bool
GenericHTTPResponse::FileAndStringBody::write(SocketBuffer &socketBuffer)
{
	if (!m_strSent)
	{
		socketBuffer.store(m_string);
		m_strSent = true;
	}

	return (true);
}

// bool
// GenericHTTPResponse::FileAndStringBody::writeFd(SocketBuffer &socketBuffer)
// {
// 	socketBuffer.store(m_fileDescriptorBuffer);

// 	return (isDone());
// }

bool
GenericHTTPResponse::FileAndStringBody::isDone(void)
{
	return (m_strSent); // a revoir
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
			if (!m_headers.empty())
			{
				socketBuffer.store(m_headers.format());
				socketBuffer.store(HTTP::CRLF);
			}

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
	FileAndStringBody *fileAndStringBody = dynamic_cast<FileAndStringBody*>(m_body);
	if (fileAndStringBody)
		out.push_back(&fileAndStringBody->fileDescriptorBuffer());
}

GenericHTTPResponse::State
GenericHTTPResponse::state() const
{
	return (m_state);
}

GenericHTTPResponse*
GenericHTTPResponse::status(HTTPStatus &status, const HTTPHeaderFields &headers)
{
	return (new GenericHTTPResponse(HTTPStatusLine(status), headers, NULL));
}

GenericHTTPResponse*
GenericHTTPResponse::file(HTTPStatus &status, const HTTPHeaderFields &headers, FileDescriptorBuffer &fileBuffer)
{
	return (new GenericHTTPResponse(HTTPStatusLine(status), headers, new GenericHTTPResponse::FileBody(fileBuffer)));
}

GenericHTTPResponse*
GenericHTTPResponse::string(HTTPStatus &status, const HTTPHeaderFields &headers, const std::string &string)
{
	return (new GenericHTTPResponse(HTTPStatusLine(status), HTTPHeaderFields(headers).contentLength(string.length()), new GenericHTTPResponse::StringBody(string)));
}

GenericHTTPResponse*
GenericHTTPResponse::fileAndString(HTTPStatus &status, const HTTPHeaderFields &headers, FileDescriptorBuffer &fileBuffer, const std::string &string)
{
	return (new GenericHTTPResponse(HTTPStatusLine(status), headers, new GenericHTTPResponse::FileAndStringBody(fileBuffer, string)));
}
