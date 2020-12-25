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

#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <exception/IOException.hpp>
#include <io/File.hpp>
#include <sys/errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>

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
File::exists()
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
	{
		int error = errno;
		errno = 0;

		if (error == ENOENT)
			return (false);

		throw IOException(m_path, errno);
	}

	return (true);
}

bool
File::isFile()
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw IOException(m_path, errno);

	return (S_ISREG(st.st_mode));
}

bool
File::isDirectory()
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw IOException(m_path, errno);

	return (S_ISDIR(st.st_mode));
}

bool
File::create(std::string location)
{
	int fd;
	// char buf[512];
	// char *current;
	// current = ::getcwd(buf, 512);
	// std::cout << "location: " << location << std::endl;
	// if (chdir(location.c_str()) == -1)
	// 	throw Exception("Location unknown");
	if((fd = (::open(m_path.c_str(), O_CREAT|O_WRONLY|O_NONBLOCK, 0666)) == -1))
		throw Exception("Could not create the ressource");
	close(fd);
	// chdir (current);
	return (true);
}

size_t
File::length()
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw IOException(m_path, errno);

	return (st.st_size);
}

std::string
File::name()
{
	std::string::size_type n = m_path.rfind('/');
	std::string::size_type pos = n == std::string::npos ? 0 : n + 1; // TODO Need fix

	return (m_path.substr(pos, m_path.length() - pos));
}

std::list<File>
File::list()
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw IOException(m_path, errno);

	if (!S_ISDIR(st.st_mode))
		throw IOException(m_path, ENOTDIR);

	DIR *dir = ::opendir(m_path.c_str());
	if (!dir)
		throw IOException(m_path, errno);

	std::list<File> files;

	struct dirent *entry;
	while ((entry = ::readdir(dir)))
		files.push_back(File(m_path + '/' + entry->d_name));

	::closedir(dir);

	return (files);
}

void
File::setNewPath(std::string path, std::string extension)
{
	std::string newName = path + '.' + extension;
	m_path = newName;
}