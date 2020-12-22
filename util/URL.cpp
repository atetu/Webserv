/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 19:02:22 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 19:02:22 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/URL.hpp>

URL::URL() :
		m_protocol(),
		m_host(),
		m_port(),
		m_path(),
		m_queryParameters(),
		m_fragment()
{
}

URL::URL(const std::string &protocol, const std::string &host, int port, const std::string &path, const Optional<std::map<std::string, std::string> > queryParameters, const Optional<std::string> fragment) :
		m_protocol(protocol),
		m_host(host),
		m_port(port),
		m_path(path),
		m_queryParameters(queryParameters),
		m_fragment(fragment)
{
}

URL::URL(const URL &other) :
		m_protocol(other.m_protocol),
		m_host(other.m_host),
		m_port(other.m_port),
		m_path(other.m_path),
		m_queryParameters(other.m_queryParameters),
		m_fragment(other.m_fragment)
{
}

URL::~URL()
{
}

const std::string&
URL::protocol() const
{
	return (m_protocol);
}

int
URL::port() const
{
	return (m_port);
}

const std::string&
URL::host() const
{
	return (m_host);
}

const std::string&
URL::path() const
{
	return (m_path);
}

const Optional<std::map<std::string, std::string> >&
URL::queryParameters() const
{
	return m_queryParameters;
}

const Optional<std::string>&
URL::fragment() const
{
	return (m_fragment);
}

URL&
URL::operator =(const URL &other)
{
	if (this != &other)
	{
		m_protocol = other.m_protocol;
		m_host = other.m_host;
		m_port = other.m_port;
		m_path = other.m_path;
		m_queryParameters = other.m_queryParameters;
		m_fragment = other.m_fragment;
	}

	return (*this);
}

bool
URL::filename(std::string &out) const
{
	if (m_path.empty())
		return (false);

	std::string::size_type lastSlashPos = m_path.rfind("/");

	if (lastSlashPos != std::string::npos)
		out = m_path.substr(lastSlashPos + 1);
	else
		out = m_path;

	return (true);
}

bool
URL::extension(std::string &out) const
{
	std::string filename;
	if (!URL::filename(filename))
		return (false);

	std::string::size_type lastDotPos = filename.rfind(".");

	if (lastDotPos == std::string::npos)
		return (false);

	out = filename.substr(lastDotPos + 1);

	return (true);
}
