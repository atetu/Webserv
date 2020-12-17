/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 18:02:12 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 18:02:12 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTOR_HPP_
# define FILEDESCRIPTOR_HPP_

#include <exception/IllegalStateException.hpp>
#include <stddef.h>
#include <sys/types.h>
#include <util/Closable.hpp>

class FileDescriptor :
		public Closable
{
	protected:
		int m_fd;
		bool m_closed;

	private:
		FileDescriptor(void);
		FileDescriptor(const FileDescriptor &other);

		FileDescriptor&
		operator=(const FileDescriptor &other);

	protected:
		inline void
		ensureNotClosed(void) const
		{
			if (m_closed)
				throw IllegalStateException("already closed");
		}

	protected:
		FileDescriptor(int fd);

	public:
		virtual
		~FileDescriptor();

		ssize_t
		read(void *buf, size_t nbyte);

		ssize_t
		write(const void *buf, size_t nbyte);

		void
		close();

		int
		raw() const;

		bool
		isClosed() const;

	public:
		static FileDescriptor*
		wrap(int fd);
};

#endif /* FILEDESCRIPTOR_HPP_ */
