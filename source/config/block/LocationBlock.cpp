/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:10:59 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/12 17:02:52 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <algorithm>

LocationBlock LocationBlock::DEFAULT;

LocationBlock::LocationBlock(void) :
		m_path(),
		m_methods(),
		m_alias(),
		m_root(),
		m_listing(),
		m_index(),
		m_cgi(),
		m_auth(),
		m_maxBodySize(-1)
{
}

LocationBlock::LocationBlock(std::string path) :
		m_path(path),
		m_methods(),
		m_alias(),
		m_root(),
		m_listing(),
		m_index(),
		m_cgi(),
		m_auth(),
		m_maxBodySize()
{
}

LocationBlock::LocationBlock(const LocationBlock &other) :
		m_path(other.m_path),
		m_methods(other.m_methods),
		m_alias(other.m_alias),
		m_root(other.m_root),
		m_listing(other.m_listing),
		m_index(other.m_index),
		m_cgi(other.m_cgi),
		m_auth(other.m_auth),
		m_maxBodySize()
{
}

LocationBlock::~LocationBlock()
{
	DeleteHelper::pointer<AuthBlock>(m_auth);
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
		m_index = other.m_index;
		m_cgi = other.m_cgi;
		m_auth = other.m_auth;
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
	m_index.set(files);

	return (*this);
}

LocationBlock&
LocationBlock::cgi(const std::string &cgi)
{
	m_cgi.set(cgi);

	return (*this);
}

LocationBlock&
LocationBlock::auth(const AuthBlock &auth)
{
	m_auth.set(&auth);

	return (*this);
}

LocationBlock&
LocationBlock::maxBodySize(const DataSize &maxBodySize)
{
	m_maxBodySize.set(maxBodySize);

	return (*this);
}

bool
LocationBlock::hasMethod(const std::string &name) const
{
	if (!m_methods.present())
		return (false);

	const std::list<std::string> &methods = m_methods.get();

	return (std::find(methods.begin(), methods.end(), name) != methods.end());
}
