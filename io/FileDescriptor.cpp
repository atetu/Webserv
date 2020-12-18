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
#include <unistd.h>
// #include <sys/types.h>
// #include <sys/uio.h>

FileDescriptor::FileDescriptor(int fd) :
		m_fd(fd),
		m_closed(false)
{
	if (::fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		throw IOException("fcntl", errno);
}

FileDescriptor::~FileDescriptor()
{
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
	return (new FileDescriptor(fd));
}
