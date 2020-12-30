/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIBlock.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 11:34:15 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/CGIBlock.hpp>
#include <sys/stat.h>
#include <errno.h>
#include <algorithm>

CGIBlock::CGIBlock() :
		m_name(),
		m_path()
{
}

CGIBlock::CGIBlock(std::string name) :
		m_name(name),
		m_path()
{
}

CGIBlock::~CGIBlock()
{
}

CGIBlock::CGIBlock(const CGIBlock &other) :
		m_name(other.m_name),
		m_path(other.m_path)
{
}

CGIBlock&
CGIBlock::operator=(const CGIBlock &other)
{
	if (this != &other)
	{
		m_name = other.m_name;
		m_path = other.m_path;
	}

	return (*this);

}

CGIBlock&
CGIBlock::path(const std::string &path)
{
	m_path.set(path);

	return (*this);
}

CGIBlock&
CGIBlock::redirectErrToOut(bool redirectErrToOut)
{
	m_redirectErrToOut.set(redirectErrToOut);

	return (*this);
}

CGIBlock&
CGIBlock::extensions(const std::list<std::string> &extensions)
{
	m_extensions.set(extensions);

	return (*this);
}

CGIBlock&
CGIBlock::environment(const std::map<std::string, std::string> &environment)
{
	m_environment.set(environment);

	return (*this);
}

bool
CGIBlock::exists(void) const
{
	if (m_path.present())
	{
		struct stat st;
		bool exists = ::stat(m_path.get().c_str(), &st) == 0;

		errno = 0;

		return (exists);
	}

	return (false);
}

bool
CGIBlock::hasExtension(const std::string &extension) const
{
	if (m_extensions.absent())
		return (true);

	const std::list<std::string> &extensions = m_extensions.get();

	return (std::find(extensions.begin(), extensions.end(), extension) != extensions.end());
}
