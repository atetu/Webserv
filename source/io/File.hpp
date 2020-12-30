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
		File(const std::string &path);
		File(const File &parent, const std::string &path);
		File(const File &parent, const File &child);
		File(const std::string &parent, const std::string &path);
		File(const File &other);

		virtual
		~File();

		File
		operator=(const File &other);

	private:
		template<typename T>
			inline static IOException
			resetErrnoAndReturn(const T &t)
			{
				errno = 0;
				return (t);
			}

		inline IOException
		ioException() const
		{
			return (resetErrnoAndReturn(IOException(m_path, errno)));
		}

	public:
		bool
		exists() const;

		bool
		isFile() const;

		bool
		isDirectory() const;

		bool
		createNewFile(mode_t mode = 0666) const;

		FileDescriptor*
		open(int flags, mode_t mode = 0) const;

		void
		remove(void) const;

		size_t
		length() const;

		std::string
		name() const;

		File
		absolute() const;

		File
		parent() const;

		inline const std::string&
		path() const
		{
			return (m_path);
		}

		std::list<File>
		list() const;

	public:
		static std::string
		concatPaths(const std::string &a, const std::string &b);

	public:
		static File
		currentDirectory();
};

#endif /* FILE_HPP_ */
