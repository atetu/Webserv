/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Environment.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 16:10:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 16:10:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/Environment.hpp>
#include <cstring>
#include <utility>

Environment::Environment() :
		m_storage()
{
}

Environment::Environment(map storage) :
		m_storage(storage)
{
}

Environment::Environment(const Environment &other) :
		m_storage(other.m_storage)
{
}

Environment::~Environment()
{
}

Environment&
Environment::operator =(const Environment &other)
{
	if (this != &other)
		m_storage = other.m_storage;

	return (*this);
}

bool
Environment::hasProperty(const std::string &key)
{
	return (m_storage.find(key) != m_storage.end());
}

bool
Environment::setProperty(const std::string &key, const std::string &value, bool onlyIfMissing)
{
	iterator it = m_storage.find(key);

	if (it == m_storage.end())
	{
		m_storage.insert(it, std::make_pair(key, value));
		return (true);
	}

	if (onlyIfMissing)
		return (false);

	it->second = value;

	return (true);
}

Environment
Environment::envp(char **envp)
{
	Environment environment;

	char *str;
	while ((str = *envp))
	{
		char *equal = std::strchr(str, '=');
		const char *value = "";

		if (equal)
		{
			equal[0] = '\0';
			value = equal + 1;
		}

		environment.setProperty(str, value);

		if (equal)
			equal[0] = '=';

		envp++;
	}

	return (environment);
}