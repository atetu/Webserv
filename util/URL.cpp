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

URL::URL(const std::string &protocol, const std::string &host, int port, const std::string &path, Optional<std::map<std::string, std::string> > queryParameters, Optional<std::string> fragment) :
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

URL
URL::parse(const std::string &protocol, const std::string host, const std::string &line)
{
	int port = 80;
	std::string path;

	bool hasQueryParameters = false;
	bool asKey = false;
	std::string parameterKey;
	std::string parameterValue;
	std::map<std::string, std::string> queryParameters;

	bool hasFragment = false;
	std::string fragment;

	std::string::const_iterator it = line.begin();
	std::string::const_iterator ite = line.end();

	while (it != ite)
	{
		char c = *it;

		if (hasQueryParameters || c == '?')
		{
			if (hasQueryParameters)
			{
				parameterKey += c;
			}

			hasQueryParameters = true;
		}

		it++;
	}

	return (URL(protocol, host, port, path, Optional<std::map<std::string, std::string> >::onlyIf(hasQueryParameters, queryParameters), Optional<std::string>::onlyIf(hasFragment, fragment)));
}
