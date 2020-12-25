/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:45 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 11:27:34 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/ServerBlock.hpp>
#include <util/helper/DeleteHelper.hpp>

const std::string ServerBlock::DEFAULT_HOST = "0.0.0.0";
const short ServerBlock::DEFAULT_PORT = 80;

ServerBlock::ServerBlock() :
		m_port(),
		m_host(),
		m_names(),
		m_maxBodySize(),
		m_root(),
		m_locations(),
		m_errors()
{
}

ServerBlock::ServerBlock(const ServerBlock &other) :
		m_port(other.m_port),
		m_host(other.m_host),
		m_names(other.m_names),
		m_maxBodySize(other.m_maxBodySize),
		m_root(other.m_root),
		m_locations(other.m_locations),
		m_errors(other.m_errors)
{
}

ServerBlock::~ServerBlock()
{
	DeleteHelper::pointers<LocationBlock>(m_locations);
}

ServerBlock&
ServerBlock::operator =(const ServerBlock &other)
{
	if (this != &other)
	{
		m_port = other.m_port;
		m_host = other.m_host;
		m_names = other.m_names;
		m_maxBodySize = other.m_maxBodySize;
		m_root = other.m_root;
		m_locations = other.m_locations;
		m_errors = (other.m_errors);
	}

	return (*this);
}

ServerBlock&
ServerBlock::port(int port)
{
	m_port.set(port);

	return (*this);
}

ServerBlock&
ServerBlock::host(std::string host)
{
	m_host.set(host);

	return (*this);
}

ServerBlock&
ServerBlock::names(const std::list<std::string> &names)
{
	m_names.set(names);

	return (*this);
}

ServerBlock&
ServerBlock::maxBodySize(const DataSize &maxBodySize)
{
	m_maxBodySize.set(maxBodySize);

	return (*this);
}

ServerBlock&
ServerBlock::root(const std::string &root)
{
	m_root.set(root);

	return (*this);
}

ServerBlock&
ServerBlock::locations(const std::list<LocationBlock const*> &locations)
{
	m_locations.set(locations);

	return (*this);
}

ServerBlock&
ServerBlock::errors(const CustomErrorMap &errors)
{
	m_errors.set(errors);

	return (*this);
}
