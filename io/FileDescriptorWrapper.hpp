/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorWrapper.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 21:26:38 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/12 21:26:38 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTORWRAPPER_HPP_
# define FILEDESCRIPTORWRAPPER_HPP_

#include <sys/types.h>
#include <string>

# define MAX_BUFFER_SIZE 4096

# include <stddef.h>
# include <exception/IOException.hpp>
# include <util/Byte.hpp>

class FileDescriptorWrapper
{
	private:
		int m_fd;
		std::string m_read_buffer;
		std::string m_write_buffer;
		bool m_finishing;

	public:
		FileDescriptorWrapper(void);
		FileDescriptorWrapper(int fd);
		FileDescriptorWrapper(const FileDescriptorWrapper &other);

		virtual
		~FileDescriptorWrapper(void);

		FileDescriptorWrapper&
		operator=(const FileDescriptorWrapper &other);

		operator int() const;

	private:
		size_t
		capacityFor(const std::string &buffer) const;

	public:

		void
		setNonBlock(void) throw (IOException);

		ssize_t
		fillWithRead(void);

		ssize_t
		fillWithReceive(void);

		ssize_t
		flushWithWrite(void);

		ssize_t
		flushWithSend(void);

		void
		store(const std::string &str);

		void
		store(const std::string &str, bool finished);

		void
		store(const char *buffer, size_t len);

		void
		store(const char *buffer, size_t len, bool finished);

		bool
		consume(char *c);

		size_t
		getReadBufferSize(void) const;

		size_t
		getReadBufferCapacity(void) const;

		size_t
		getWriteBufferSize(void) const;

		size_t
		getWriteBufferCapacity(void) const;

	public:
		static FileDescriptorWrapper*
		wrap(int fd);
};

#endif /* FILEDESCRIPTORWRAPPER_HPP_ */
