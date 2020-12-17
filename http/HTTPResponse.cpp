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
#include <http/HTTPResponse.hpp>
#include <util/buffer/impl/BaseBuffer.hpp>
#include <util/buffer/impl/FileBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <iostream>
#include <sstream>

HTTPResponse::StatusLine::StatusLine(void) :
		m_version(HTTPVersion::HTTP_1_1),
		m_status()
{
}

HTTPResponse::StatusLine::StatusLine(const HTTPStatus &status) :
		m_version(HTTPVersion::HTTP_1_1),
		m_status(status)
{
}

HTTPResponse::StatusLine::StatusLine(const HTTPVersion &version, const HTTPStatus &status) :
		m_version(version),
		m_status(status)
{
}

HTTPResponse::StatusLine::StatusLine(const StatusLine &other) :
		m_version(other.m_version),
		m_status(other.m_status)
{
}

HTTPResponse::StatusLine::~StatusLine()
{
}

HTTPResponse::StatusLine&
HTTPResponse::StatusLine::operator =(const StatusLine &other)
{
	if (this != &other)
	{
		m_version = other.m_version;
		m_status = other.m_status;
	}

	return (*this);
}

std::string
HTTPResponse::StatusLine::format(void) const
{
	std::stringstream stream;

	stream << m_version.format() << HTTP::SP << m_status.code() << HTTP::SP << m_status.reasonPhrase();

	return (stream.str());
}

HTTPResponse::IBody::~IBody()
{
}

HTTPResponse::FileBody::FileBody(FileBuffer &fileBuffer) :
		m_fileBuffer(fileBuffer)
{
}

FileBuffer&
HTTPResponse::FileBody::fileBuffer()
{
	return (m_fileBuffer);
}

HTTPResponse::FileBody::~FileBody()
{
}

bool
HTTPResponse::FileBody::write(SocketBuffer &socketBuffer)
{
	socketBuffer.storeFrom(m_fileBuffer);

	return (isDone());
}

bool
HTTPResponse::FileBody::isDone(void)
{
	return (m_fileBuffer.hasReadEverything() && m_fileBuffer.size() == 0);
}

HTTPResponse::StringBody::StringBody(std::string string) :
		m_string(string),
		m_sent(false)
{
}

HTTPResponse::StringBody::~StringBody()
{
	std::cout << "~StringBody(): m_sent=" << m_sent << std::endl;
}

bool
HTTPResponse::StringBody::write(SocketBuffer &socketBuffer)
{
	if (!m_sent)
	{
		socketBuffer.store(m_string);
		m_sent = true;
	}

	return (true);
}

bool
HTTPResponse::StringBody::isDone(void)
{
	return (m_sent);
}

HTTPResponse::HTTPResponse(const HTTPStatus &status, const HTTPHeaderFields &headers, IBody *body) :
		m_statusLine(HTTPVersion::HTTP_1_1, status),
		m_headers(headers),
		m_body(body),
		m_state(NONE),
		m_state_index(-1)
{
}

HTTPResponse::HTTPResponse(const HTTPVersion &version, const HTTPStatus &status, const HTTPHeaderFields &headers, IBody *body) :
		m_statusLine(version, status),
		m_headers(headers),
		m_body(body),
		m_state(NONE),
		m_state_index(-1)
{
}

HTTPResponse::~HTTPResponse()
{
	std::cout << "~HTTPResponse(): m_body=" << (void*)m_body << std::endl;

	if (m_body)
		delete m_body;
}

bool
HTTPResponse::write(SocketBuffer &socketBuffer)
{
	std::string str;
	HTTPHeaderFields::const_iterator it;
	HTTPHeaderFields::const_iterator ite;

	if (m_state == NONE)
		m_state = HEADERS;

	switch (m_state)
	{
		case HEADERS:
			socketBuffer.store(m_statusLine.format());
			socketBuffer.store(HTTP::CRLF);
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
			if (socketBuffer.send() < 0 || socketBuffer.size() == 0)
				m_state = FINISHED;

			break;

		default:
			break;
	}

	return (m_state == FINISHED);
}
