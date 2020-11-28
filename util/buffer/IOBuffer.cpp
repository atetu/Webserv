/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOBuffer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 22:38:35 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 22:38:35 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/IOException.hpp>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <util/buffer/IOBuffer.hpp>
#include <algorithm>
#include <string>
#include <fcntl.h>

IOBuffer::IOBuffer() :
		BaseBuffer(),
		m_fd(-1),
		m_maxSize(std::string::npos)
{
}

IOBuffer::IOBuffer(int fd, size_t maxSize) :
		BaseBuffer(),
		m_fd(fd),
		m_maxSize(maxSize)
{
	if (::fcntl(m_fd, F_SETFL, O_NONBLOCK) == -1)
		throw IOException("fcntl", errno);
}

IOBuffer::IOBuffer(const IOBuffer &other) :
		BaseBuffer(other),
		m_fd(other.m_fd),
		m_maxSize(other.m_maxSize)
{
}

IOBuffer::~IOBuffer()
{
}

IOBuffer&
IOBuffer::operator =(const IOBuffer &other)
{
	BaseBuffer::operator =(other);

	if (this != &other)
		m_fd = other.m_fd;

	return (*this);
}

ssize_t
IOBuffer::read(size_t len)
{
	size_t capacity = this->capacity();

	if (m_maxSize == std::string::npos)
		capacity = len;

	if (capacity)
	{
		char rbuffer[capacity];
		ssize_t r = ::read(m_fd, rbuffer, std::min(capacity, len));

		if (r >= 0)
			store(rbuffer, r);

		return (r);
	}

	return (0);
}

ssize_t
IOBuffer::recv(size_t len)
{
	size_t capacity = this->capacity();

	if (m_maxSize == std::string::npos)
		capacity = len;

	if (capacity)
	{
		char rbuffer[capacity];
		ssize_t r = ::recv(m_fd, rbuffer, std::min(capacity, len), 0);

		if (r >= 0)
			store(rbuffer, r);

		return (r);
	}

	return (0);
}

ssize_t
IOBuffer::write(size_t len)
{
	ssize_t r = ::write(m_fd, m_buffer.data(), std::min(m_buffer.length(), len));

	if (r > 0)
		m_buffer.erase(0, r);

	return (r);
}

ssize_t
IOBuffer::send(size_t len)
{
	ssize_t r = ::send(m_fd, m_buffer.data(), std::min(m_buffer.length(), len), 0);

	if (r > 0)
		m_buffer.erase(0, r);

	return (r);
}

size_t
IOBuffer::capacity()
{
	if (m_buffer.size() >= m_maxSize)
		return (0);

	return (m_maxSize - m_buffer.size());
}
