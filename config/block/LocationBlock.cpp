/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:10:59 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/15 16:32:11 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>

LocationBlock LocationBlock::DEFAULT;

LocationBlock::LocationBlock(void) :
		m_path(),
		m_methods(),
		m_alias(),
		m_root(),
		m_listing(),
		m_indexFiles(),
		m_cgi()
{
}

LocationBlock::LocationBlock(std::string path) :
		m_path(path),
		m_methods(),
		m_alias(),
		m_root(),
		m_listing(),
		m_indexFiles(),
		m_cgi()
{
}

LocationBlock::LocationBlock(const LocationBlock &other) :
		m_path(other.m_path),
		m_methods(other.m_methods),
		m_alias(other.m_alias),
		m_root(other.m_root),
		m_listing(other.m_listing),
		m_indexFiles(other.m_indexFiles),
		m_cgi(other.m_cgi)
{
}

LocationBlock::~LocationBlock()
{
}

LocationBlock&
LocationBlock::operator =(const LocationBlock &other)
{
	if (this != &other)
	{
		m_path = other.m_path;
		m_methods = other.m_methods;
		m_alias = other.m_alias;
		m_root = other.m_root;
		m_listing = other.m_listing;
		m_indexFiles = other.m_indexFiles;
		m_cgi = other.m_cgi;
	}

	return (*this);
}

LocationBlock&
LocationBlock::methods(const std::list<std::string> &methods)
{
	m_methods.set(methods);

	return (*this);
}

LocationBlock&
LocationBlock::alias(const std::string &alias)
{
	m_alias.set(alias);

	return (*this);
}

LocationBlock&
LocationBlock::root(const std::string &root)
{
	m_root.set(root);

	return (*this);
}

LocationBlock&
LocationBlock::listing(bool listing)
{
	m_listing.set(listing);

	return (*this);
}

LocationBlock&
LocationBlock::index(const std::list<std::string> &files)
{
	m_indexFiles.set(files);

	return (*this);
}

LocationBlock&
LocationBlock::cgi(const std::string &cgi)
{
	m_cgi.set(cgi);

	return (*this);
}
