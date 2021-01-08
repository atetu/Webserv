/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 18:02:12 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 18:02:12 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/IOException.hpp>
#include <io/FileDescriptor.hpp>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>

FileDescriptor::FileDescriptor(int fd) :
		m_fd(fd),
		m_closed(false)
{
}

FileDescriptor::~FileDescriptor()
{
	if (!m_closed)
		::close(m_fd);
}

ssize_t
FileDescriptor::read(void *buf, size_t nbyte)
{
	ensureNotClosed();

	return (::read(m_fd, buf, nbyte));
}

ssize_t
FileDescriptor::write(const void *buf, size_t nbyte)
{
	ensureNotClosed();

	return (::write(m_fd, buf, nbyte));
}

void
FileDescriptor::close()
{
	ensureNotClosed();

	if (!m_closed)
	{
		::close(m_fd);

		m_fd = -1;
		m_closed = true;
	}
}

void
FileDescriptor::nonBlocking()
{
	if (::fcntl(m_fd, F_SETFL, O_NONBLOCK) == -1)
		throw IOException("fcntl", errno);
}

int
FileDescriptor::raw() const
{
	return (m_fd);
}

bool
FileDescriptor::isClosed() const
{
	return (m_closed);
}

FileDescriptor*
FileDescriptor::wrap(int fd)
{
	FileDescriptor *fileDescriptor = new FileDescriptor(fd);

	try
	{
		fileDescriptor->nonBlocking();
	}
	catch (...)
	{
		delete fileDescriptor;

		throw;
	}

	return (fileDescriptor);
}
