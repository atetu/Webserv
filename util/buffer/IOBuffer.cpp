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
		m_closeOnDestroy(false),
		m_maxSize(std::string::npos),
		m_readEverything(false)
{
}

IOBuffer::IOBuffer(int fd, bool closeOnDestroy, size_type maxSize) :
		BaseBuffer(),
		m_fd(fd),
		m_closeOnDestroy(closeOnDestroy),
		m_maxSize(maxSize),
		m_readEverything(false)
{
	if (::fcntl(m_fd, F_SETFL, O_NONBLOCK) == -1)
		throw IOException("fcntl", errno);
}

IOBuffer::IOBuffer(const IOBuffer &other) :
		BaseBuffer(other),
		m_fd(other.m_fd),
		m_closeOnDestroy(other.m_closeOnDestroy),
		m_maxSize(other.m_maxSize),
		m_readEverything(other.m_readEverything)
{
}

IOBuffer::~IOBuffer()
{
	if (m_closeOnDestroy)
		close();
}

IOBuffer&
IOBuffer::operator =(const IOBuffer &other)
{
	if (this != &other)
	{
		BaseBuffer::operator =(other);

		m_fd = other.m_fd;
		m_closeOnDestroy = other.m_closeOnDestroy;
		m_maxSize = other.m_maxSize;
		m_readEverything = other.m_readEverything;
	}

	return (*this);
}

ssize_t
IOBuffer::read(size_t len)
{
	size_t capacity;

	if (m_maxSize == std::string::npos)
		capacity = len;
	else
		capacity = this->capacity();

	if (capacity)
	{
		char rbuffer[capacity];
		ssize_t r = ::read(m_fd, rbuffer, std::min(capacity, len));

		if (r >= 0)
			BaseBuffer::store(rbuffer, r);

		m_readEverything = r == 0;

		return (r);
	}

	return (0);
}

ssize_t
IOBuffer::recv(size_t len)
{
	size_t capacity;

	if (m_maxSize == std::string::npos)
		capacity = len;
	else
		capacity = this->capacity();

	if (capacity)
	{
		char rbuffer[capacity];
		ssize_t r = ::recv(m_fd, rbuffer, std::min(capacity, len), 0);

		if (r >= 0)
			BaseBuffer::store(rbuffer, r);

		m_readEverything = r == 0;

		return (r);
	}

	return (0);
}

ssize_t
IOBuffer::write(size_t len)
{
	ssize_t r = ::write(m_fd, m_storage.data(), std::min(m_storage.length(), len));

	if (r > 0)
		m_storage.erase(0, r);

	return (r);
}

ssize_t
IOBuffer::send(size_t len)
{
	ssize_t r = ::send(m_fd, m_storage.data(), std::min(m_storage.length(), len), 0);

	if (r > 0)
		m_storage.erase(0, r);

	return (r);
}

size_t
IOBuffer::capacity() const
{
	if (m_storage.size() >= m_maxSize)
		return (0);

	return (m_maxSize - m_storage.size());
}

void
IOBuffer::close(void)
{
	::close(m_fd);

	m_fd = -1;
}
