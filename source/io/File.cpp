/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 23:36:43 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/11 16:26:43 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <exception/IllegalStateException.hpp>
#include <io/File.hpp>
#include <io/FileDescriptor.hpp>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <util/StringUtils.hpp>
#include <cstddef>
#include <iostream>

File::File() :
		m_path()
{
}

File::File(const std::string &path) :
		m_path(path)
{
}

File::File(const File &parent, const std::string &path) :
		m_path(concatPaths(parent.path(), path))
{
}

File::File(const File &parent, const File &child) :
		m_path(concatPaths(parent.path(), child.path()))
{
}

File::File(const std::string &parent, const std::string &path) :
		m_path(concatPaths(parent, path))
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
File::createNewFile(mode_t mode) const
{
	int fd;

	// std::cout << "path: " <<  m_path << std::endl; // TODO split the path ind directories and sub directories
	std::string copy = m_path;
	std::string previousStr = "";
	std::size_t found;
	//found = copy.find('/');
	while ((found = copy.find('/')) != std::string::npos)
	{
		// std::cout << "found: " << found << std::endl;
		std::string newPath = copy.substr(0, found);
		std::string remainedPath = copy.substr(found + 1, std::string::npos);
		if (newPath == "./" || newPath == "/")
		{
			previousStr = newPath;
			copy = remainedPath;
			//	found = copy.find('/');
		}
		else
		{
			// std::cout << "new: " <<  newPath <<std::endl;
			// std::cout << "rem: " <<  remainedPath <<std::endl;
			File file(previousStr + "/" + newPath);
			if (!file.exists())
			{
				// std::cout << "new file: " << file.path() << std::endl;
				mkdir(file.path().c_str(), 0777);
			}
			previousStr = newPath;
			copy = remainedPath;
			//	found = copy.find('/');

		}

	}
	// mkdir ("put_test/test",0777);
	if ((fd = ::open(m_path.c_str(), O_CREAT, mode)) == -1) //TOTO create directory if needed
	{
		errno = 0;
		//	std::cout << "error\n";
		return (false); //TODO handle errors
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

Time
File::lastModified() const
{
	struct stat st;
	if (::stat(m_path.c_str(), &st) == -1)
		throw ioException();

	return (Time(st.st_mtime));
}

std::string
File::name() const
{
	std::string::size_type n = m_path.rfind('/');
	std::string::size_type pos = n == std::string::npos ? 0 : n + 1; // TODO Need fix

	return (m_path.substr(pos, m_path.length() - pos));
}

FileDescriptor*
File::open(int flags, mode_t mode) const
{
	int fd;
	if ((fd = ::open(m_path.c_str(), flags, mode)) == -1)
		throw ioException();

	try
	{
		return (FileDescriptor::wrap(fd));
	}
	catch (...)
	{
		::close(fd);

		throw;
	}
}

void
File::remove(void) const
{
	if (isDirectory())
		::rmdir(m_path.c_str());
	else if (isFile())
		::unlink(m_path.c_str());
	else
		throw IllegalStateException(m_path + ": not a directory or a file");
}

bool
File::tryRemove(void) const
{
	try
	{
		remove();

		return (true);
	}
	catch (Exception &exception)
	{
		return (false);
	}
}

File
File::absolute() const
{
	return (currentDirectory().path() + "/" + m_path);
}

File
File::parent() const
{
	if (m_path.empty())
		return (std::string("/"));

	std::string::size_type stopAt = std::string::npos;

	if (StringUtils::last(m_path) == '/')
	{
		stopAt = 0;

		for (std::string::size_type n = m_path.size(); n != 0; n--)
		{
			std::string::size_type index = n - 1;

			if (m_path[index] != '/')
			{
				stopAt = index;
				break;
			}
		}
	}

	std::string::size_type lastAt;
	if (stopAt == 0 || (lastAt = m_path.rfind('/', stopAt)) == 0)
		return (std::string("/")); /* root */

	if (lastAt == std::string::npos)
		return (m_path + "/../");

	return (m_path.substr(0, lastAt));
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

std::string
File::concatPaths(const std::string &a, const std::string &b)
{
	bool aEnd = StringUtils::last(a) == '/';
	bool bStart = StringUtils::first(b) == '/';

	if (aEnd && bStart)
		return (a + b.substr(1));

	if (aEnd ^ bStart)
		return (a + b);

	return (a + '/' + b);
}

bool
File::findFilename(const std::string &path, std::string &out)
{
	if (path.empty())
		return (false);

	std::string::size_type lastSlashPos = path.rfind("/");

	if (lastSlashPos != std::string::npos)
		out = path.substr(lastSlashPos + 1);
	else
		out = path;

	return (true);
}

bool
File::findExtension(const std::string &path, std::string &out)
{
	std::string filename;
	if (!File::findFilename(path, filename))
		return (false);

	std::string::size_type lastDotPos = filename.rfind(".");

	if (lastDotPos == std::string::npos)
		return (false);

	out = filename.substr(lastDotPos + 1);

	return (true);
}

bool
File::findMime(std::string &out, HTTPRequest &request)
{
	const MimeRegistry &mimeRegistry = Configuration::instance().mimeRegistry();
	std::string type = request.headers().get(HTTPHeaderFields::CONTENT_TYPE).orElse("");
	//std::cout << "path inside mime: " << m_path << std::endl;
	if (!type.empty())
	{
		const Mime *mime = mimeRegistry.findByMimeType(type);
		if (mime == NULL)
		{
			//LOG.warn() << "Extension conversion not handled (1)" << std::endl;
			return (false);
		}

		// std::string path = file.path();
		std::size_t found = m_path.find_last_of(out);
				
		if (!out.empty())
		{
			Mime::iterator ext_it = std::find(mime->extensions().begin(), mime->extensions().end(), out);
			if (ext_it != mime->extensions().end())
				; // TODO Bad conditional
			else if (ext_it == mime->extensions().end() && exists())
			{
			//	std::cout << "not handled\n";
				//LOG.warn() << "Extension conversion not handled(2)" << std::endl;
				return (false);
			}
			else
			{
			//	std::cout << "right\n";
				m_path = m_path.substr(0, found) + "." + *(mime->extensions().begin());
			}
		}
		
		else if (!exists())
			m_path = m_path + "." + *(mime->extensions().begin());
	}
//	std::cout << "path: "  << m_path << std::endl;
	return (true);
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
