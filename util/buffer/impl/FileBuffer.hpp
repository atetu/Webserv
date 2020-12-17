/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileBuffer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 18:24:26 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 18:24:26 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEBUFFER_HPP_
# define FILEBUFFER_HPP_

#include <io/FileDescriptor.hpp>
#include <stddef.h>
#include <sys/types.h>
#include <util/buffer/impl/BaseBuffer.hpp>
#include <string>

class FileBuffer :
		public BaseBuffer,
		public Closable
{
	public:
		enum ActionOnDestructorMode
		{
			NOTHING/**/= 0b00,
			CLOSE/*  */= 0b01,
			DELETE/* */= 0b10,
		};

	protected:
		FileDescriptor &m_fd;
		int m_actionOnDestroy;
		size_type m_maxSize;
		bool m_readEverything;

	public:
		FileBuffer(void);
		FileBuffer(const FileBuffer &other);

		FileBuffer&
		operator =(const FileBuffer &other);

	protected:
		FileBuffer(FileDescriptor &fileDescriptor, int actionOnDestroy, size_type maxSize);

	public:
		virtual
		~FileBuffer();

		ssize_t
		read(size_t len = 2048);

		ssize_t
		write(size_t len = 2048);

		void
		storeFrom(FileBuffer &buffer, bool andClear = true);

		size_t
		capacity() const;

		void
		close(void);

		inline bool
		hasReadEverything() const
		{
			return (m_readEverything);
		}

		inline const FileDescriptor&
		descriptor(void)
		{
			return (m_fd);
		}

	public:
		static FileBuffer*
		from(FileDescriptor &fileDescriptor, int actionOnDestroy = CLOSE, size_type maxSize = std::string::npos);

};

#endif /* FILEBUFFER_HPP_ */
