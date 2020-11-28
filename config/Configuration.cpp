/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 11:33:47 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Configuration.hpp>

Configuration::Configuration() :
		m_mimeRegistry(),
		m_servers()
{
}

Configuration::Configuration(const MimeRegistry &mimeRegistry, const std::vector<ServerBlock> &servers) :
		m_mimeRegistry(mimeRegistry),
		m_servers(servers)
{
}

Configuration::Configuration(const Configuration &other) :
		m_mimeRegistry(other.m_mimeRegistry),
		m_servers(other.m_servers)
{
}

Configuration::~Configuration()
{
}

Configuration&
Configuration::operator=(const Configuration &other)
{
	if (this != &other)
		m_servers = other.m_servers;

	return (*this);
}

const std::vector<ServerBlock>&
Configuration::servers(void) const
{
	return (m_servers);
}
