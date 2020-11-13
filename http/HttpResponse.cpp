/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 16:38:46 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/27 16:38:46 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <io/FileDescriptorWrapper.hpp>

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
#include <http/HttpResponse.hpp>
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

HttpResponse::StatusLine::StatusLine(void) :
		m_version(HTTPVersion::HTTP_1_1),
		m_status()
{
}

HttpResponse::StatusLine::StatusLine(const HTTPStatus &status) :
		m_version(HTTPVersion::HTTP_1_1),
		m_status(status)
{
}

HttpResponse::StatusLine::StatusLine(const HTTPVersion &version, const HTTPStatus &status) :
		m_version(version),
		m_status(status)
{
}

HttpResponse::StatusLine::StatusLine(const StatusLine &other) :
		m_version(other.m_version),
		m_status(other.m_status)
{
}

HttpResponse::StatusLine::~StatusLine()
{
}

HttpResponse::StatusLine&
HttpResponse::StatusLine::operator =(const StatusLine &other)
{
	if (this != &other)
	{
		m_version = other.m_version;
		m_status = other.m_status;
	}

	return (*this);
}

std::string
HttpResponse::StatusLine::format(void) const
{
	std::stringstream stream;

	stream << m_version.format() << HTTP::SP << m_status.code() << HTTP::SP << m_status.reasonPhrase();

	return (stream.str());
}

HttpResponse::IBody::~IBody()
{
}

HttpResponse::FileBody::FileBody(int fd) :
		m_fd(fd)
{
}

HttpResponse::FileBody::~FileBody()
{
	if (m_fd != -1)
		::close(m_fd);
	std::cout << "~ closed fd #" << m_fd << std::endl;
}

bool
HttpResponse::FileBody::write(FileDescriptorWrapper &fd)
{
	if (m_fd == -1)
		return (1);

	size_t capacity = fd.getReadBufferCapacity();

	if (capacity)
	{
		char buffer[capacity];
		ssize_t r = ::read(m_fd, buffer, capacity);

		if (r == 0)
		{
			std::cout << "closed fd #" << m_fd << std::endl;
			::close(m_fd);
			m_fd = -1;
		}

		if (r >= 0)
			fd.store(buffer, capacity, m_fd == -1);
	}

	return (m_fd == -1);
}

HttpResponse::StringBody::StringBody(std::string string) :
		m_string(string),
		m_sent(false)
{
}

HttpResponse::StringBody::~StringBody()
{
}

bool
HttpResponse::StringBody::write(FileDescriptorWrapper &fd)
{
	if (!m_sent)
	{
		fd.store(m_string, true);
		m_sent = true;
	}

	return (1);
}

HttpResponse::HttpResponse(const HTTPVersion &version, const HTTPStatus &status, HTTPHeaderFields headers, IBody *body) :
		m_statusLine(version, status),
		m_headers(headers),
		m_body(body),
		m_state(NONE),
		m_state_index(-1)
{
}

HttpResponse::~HttpResponse()
{
	if (m_body)
		delete m_body;
}

bool
HttpResponse::write(FileDescriptorWrapper &fd)
{
	std::string str;
	HTTPHeaderFields::const_iterator it;
	HTTPHeaderFields::const_iterator ite;

	if (m_state == NONE)
		m_state = HEADERS;

//	std::cout << (int)fd << ": " << m_state << std::endl;

	switch (m_state)
	{
		case HEADERS:
			fd.store(m_statusLine.format());
			fd.store(HTTP::CRLF);
			fd.store(m_headers.format());
			fd.store(HTTP::CRLF);

			if (m_body)
				m_state = BODY;
			else
				m_state = FLUSHING;

			break;

		case BODY:
			if (m_body && m_body->write(fd))
				m_state = FLUSHING;

			break;

		case FLUSHING:
			if (fd.flushWithSend() < 0 || fd.getWriteBufferSize() == 0)
				m_state = FINISHED;

			break;

		default:
			break;
	}

	return (m_state == FINISHED);
}
