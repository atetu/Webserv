/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 21:26:38 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/12 21:26:38 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/IOException.hpp>
#include <io/FileDescriptorWrapper.hpp>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <algorithm>

#ifdef __linux__
#include <fcntl.h>
#elif __APPLE__
#include <fcntl.h>
#elif __CYGWIN__
#include <sys/_default_fcntl.h>
#else
# error Unknown plateform
#endif

FileDescriptorWrapper::FileDescriptorWrapper(void) :
		m_fd(-1),
		m_read_buffer(),
		m_write_buffer(),
		m_finishing(false)
{
	m_read_buffer.reserve(MAX_BUFFER_SIZE);
	m_write_buffer.reserve(MAX_BUFFER_SIZE);
}

FileDescriptorWrapper::FileDescriptorWrapper(int fd) :
		m_fd(fd),
		m_read_buffer(),
		m_write_buffer(),
		m_finishing(false)
{
	m_read_buffer.reserve(MAX_BUFFER_SIZE);
	m_write_buffer.reserve(MAX_BUFFER_SIZE);
}

FileDescriptorWrapper::FileDescriptorWrapper(const FileDescriptorWrapper &other) :
		m_fd(other.m_fd),
		m_read_buffer(other.m_read_buffer),
		m_write_buffer(other.m_write_buffer),
		m_finishing(other.m_finishing)
{
}

FileDescriptorWrapper::~FileDescriptorWrapper(void)
{
}

FileDescriptorWrapper&
FileDescriptorWrapper::operator=(const FileDescriptorWrapper &other)
{
	if (this != &other)
	{
		m_fd = other.m_fd;
		m_read_buffer = other.m_read_buffer;
		m_write_buffer = other.m_write_buffer;
		m_finishing = other.m_finishing;
	}

	return (*this);
}

FileDescriptorWrapper::operator int() const
{
	return (m_fd);
}

size_t
FileDescriptorWrapper::capacityFor(const std::string &buffer) const
{
	if (buffer.size() >= MAX_BUFFER_SIZE)
		return (0);

	return (MAX_BUFFER_SIZE - buffer.size());
}

void
FileDescriptorWrapper::setNonBlock(void) throw (IOException)
{
	if (::fcntl(m_fd, F_SETFL, O_NONBLOCK) == -1)
		throw IOException("fcntl", errno);
}

ssize_t
FileDescriptorWrapper::fillWithRead(void)
{
	size_t capacity = getReadBufferCapacity();

	if (capacity)
	{
		char rbuffer[capacity + 1];
		ssize_t r = ::read(m_fd, rbuffer, capacity);

		if (r >= 0)
		{
			rbuffer[r] = '\0';
			m_read_buffer += static_cast<char*>(rbuffer);
		}

		m_finishing = r == 0;

		return (r);
	}

	return (0);
}

ssize_t
FileDescriptorWrapper::fillWithReceive(void)
{
	size_t capacity = getReadBufferCapacity();

	if (capacity)
	{
		char rbuffer[capacity + 1];
		ssize_t r = ::recv(m_fd, rbuffer, capacity, 0);

		if (r >= 0)
		{
			rbuffer[r] = '\0';
			m_read_buffer += static_cast<char*>(rbuffer);
		}

		m_finishing = r == 0;

		return (r);
	}

	return (0);
}

ssize_t
FileDescriptorWrapper::flushWithWrite(void)
{
	ssize_t r = ::write(m_fd, m_write_buffer.data(), m_write_buffer.length());

	if (r > 0)
		m_write_buffer.erase(0, r);

	return (r);
}

ssize_t
FileDescriptorWrapper::flushWithSend(void)
{
	ssize_t r = ::send(m_fd, m_write_buffer.data(), m_write_buffer.length(), 0);

	if (r > 0)
		m_write_buffer.erase(0, r);

	return (r);
}

void
FileDescriptorWrapper::store(const std::string &str)
{
	store(str, false);
}

void
FileDescriptorWrapper::store(const std::string &str, bool finished)
{
	m_write_buffer += str;
	m_finishing = finished;
}

void
FileDescriptorWrapper::store(const char *buffer, size_t len)
{
	store(buffer, len, false);
}

void
FileDescriptorWrapper::store(const char *buffer, size_t len, bool finished)
{
	m_write_buffer.append(buffer, len);
	m_finishing = finished;
}

bool
FileDescriptorWrapper::consume(char *c)
{
	if (m_read_buffer.empty())
		return (false);

	*c = m_read_buffer[0];

	m_read_buffer.erase(0, 1);

	return (true);
}

size_t
FileDescriptorWrapper::getReadBufferSize(void) const
{
	return (m_read_buffer.size());
}

size_t
FileDescriptorWrapper::getReadBufferCapacity(void) const
{
	return (capacityFor(m_read_buffer));
}

size_t
FileDescriptorWrapper::getWriteBufferSize(void) const
{
	return (m_write_buffer.size());
}

size_t
FileDescriptorWrapper::getWriteBufferCapacity(void) const
{
	return (capacityFor(m_write_buffer));
}

bool
FileDescriptorWrapper::isFinishing(void)
{
	return (m_finishing);
}

bool
FileDescriptorWrapper::isDone(void)
{
	return (m_finishing && m_write_buffer.empty() && m_read_buffer.empty());
}

FileDescriptorWrapper*
FileDescriptorWrapper::wrap(int fd)
{
	return (new FileDescriptorWrapper(fd));
}
