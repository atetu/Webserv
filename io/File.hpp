/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 23:36:43 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 18:40:48 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP_
# define FILE_HPP_

#include <exception/IOException.hpp>
#include <stddef.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <list>
#include <string>

class FileDescriptor;

class File
{
	private:
		std::string m_path;

	public:
		File();
		File(std::string path);
		File(const File &other);

		virtual
		~File();

		File
		operator=(const File &other);

		bool
		exists() const;

		bool
		isFile() const;

		bool
		isDirectory() const;

		bool
		create(mode_t mode = 0666) const;

		FileDescriptor*
		open(int flags, mode_t mode = 0);

		size_t
		length() const;

		std::string
		name() const;

		inline const std::string&
		path()
		{
			return (m_path);
		}

		std::list<File>
		list() const;

	private:
		inline IOException
		ioException() const
		{
			int err = errno;
			errno = 0;
			return (IOException(m_path, err));
		}
};

#endif /* FILE_HPP_ */
