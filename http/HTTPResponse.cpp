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

#ifdef __linux__
# include <sys/socket.h>
# include <unistd.h>
#elif __APPLE__
# include <sys/socket.h>
# include <unistd.h>
#elif __CYGWIN__
# include <cygwin/socket.h>
# include <sys/unistd.h>
#else
# error Unknown plateform
#endif

#include <exception/IOException.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTP.hpp>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <cstring>
#include <iostream>
#include <map>

#if 1
#include <sstream>
#endif

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

HTTPResponse::FileBody::FileBody(int fd) :
		m_buffer(fd, true)
{
}

IOBuffer&
HTTPResponse::FileBody::buffer()
{
	return (m_buffer);
}

HTTPResponse::FileBody::~FileBody()
{
}

bool
HTTPResponse::FileBody::write(IOBuffer &buffer)
{
	buffer.storeFrom(m_buffer);

	return (m_buffer.hasReadEverything() && m_buffer.capacity() == 0);
}

HTTPResponse::StringBody::StringBody(std::string string) :
		m_string(string),
		m_sent(false)
{
}

HTTPResponse::StringBody::~StringBody()
{
}

bool
HTTPResponse::StringBody::write(IOBuffer &fd)
{
	if (!m_sent)
	{
		fd.store(m_string);
		m_sent = true;
	}

	return (true);
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
	if (m_body)
		delete m_body;
}

bool
HTTPResponse::write(IOBuffer &out)
{
	std::string str;
	HTTPHeaderFields::const_iterator it;
	HTTPHeaderFields::const_iterator ite;

	if (m_state == NONE)
		m_state = HEADERS;

	switch (m_state)
	{
		case HEADERS:
			out.store(m_statusLine.format());
			out.store(HTTP::CRLF);
			out.store(m_headers.format());
			out.store(HTTP::CRLF);

			if (m_body)
				m_state = BODY;
			else
				m_state = FLUSHING;

			break;

		case BODY:
			if (m_body && m_body->write(out))
				m_state = FLUSHING;

			break;

		case FLUSHING:
			if (out.send() < 0 || out.capacity() == 0)
				m_state = FINISHED;

			break;

		default:
			break;
	}

	return (m_state == FINISHED);
}