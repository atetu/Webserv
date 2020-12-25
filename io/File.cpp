/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 23:36:43 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/25 10:23:31 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <io/File.hpp>
#include <io/FileDescriptor.hpp>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/unistd.h>

File::File() :
		m_path()
{
}

File::File(std::string path) :
		m_path(path)
{
}

File::File(const File &other) :
		m_path(other.m_path)
{
}

File::~File()
{
}

File
File::operator =(const File &other)
{
	if (this != &other)
		m_path = other.m_path;

	return (*this);
}

bool
File::exists() const
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
	{
		int error = errno;
		errno = 0;

		if (error == ENOENT)
			return (false);

		throw ioException();
	}

	return (true);
}

bool
File::isFile() const
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw ioException();

	return (S_ISREG(st.st_mode));
}

bool
File::isDirectory() const
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw ioException();

	return (S_ISDIR(st.st_mode));
}

bool
File::create(mode_t mode) const
{
	int fd;
	if ((fd = ::open(m_path.c_str(), O_CREAT, mode)) == -1)
	{
		errno = 0;
		return (false);
	}

	close(fd);

	return (true);
}

size_t
File::length() const
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw ioException();

	return (st.st_size);
}

std::string
File::name() const
{
	std::string::size_type n = m_path.rfind('/');
	std::string::size_type pos = n == std::string::npos ? 0 : n + 1; // TODO Need fix

	return (m_path.substr(pos, m_path.length() - pos));
}

FileDescriptor*
File::open(int flags, mode_t mode)
{
	int fd;
	if ((fd = ::open(m_path.c_str(), flags, mode)) == -1)
		throw ioException();

	return (FileDescriptor::wrap(fd));
}

File
File::absolute() const
{
	return (currentDirectory().path() + "/" + m_path);
}

std::list<File>
File::list() const
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw ioException();

	if (!S_ISDIR(st.st_mode))
		throw IOException(m_path, ENOTDIR);

	DIR *dir = ::opendir(m_path.c_str());
	if (!dir)
		throw ioException();

	std::list<File> files;

	struct dirent *entry;
	while ((entry = ::readdir(dir)))
		files.push_back(File(m_path + '/' + entry->d_name));

	::closedir(dir);

	return (files);
}

File
File::currentDirectory()
{
	char *cwd = ::getcwd(NULL, 0);

	if (!cwd)
		throw resetErrnoAndReturn(IOException("getcwd", errno));

	std::string str = cwd;
	free(cwd);

	return (File(str));
}
