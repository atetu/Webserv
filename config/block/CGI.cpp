/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 11:34:15 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/CGI.hpp>

CGI::CGI():
	m_name(""),
	m_path()
{
}

CGI::CGI(std::string name):
	m_name(name),
	m_path()
{
}

CGI::~CGI()
{
}

CGI::CGI(const CGI &other) :
	m_name(other.m_name),
	m_path(other.m_path)
{
}

CGI&
CGI::operator=(const CGI &other)
{
	if (this != &other)
	{
		m_name = other.m_name;
		m_path = other.m_path;
	}

	return (*this);

}

CGI&
CGI::path(const std::string & path)
{
	m_path.set(path);

	return (*this);
}

const std::string
CGI::name(void) const
{
	return (m_name);
}

const Optional<std::string>
CGI::path(void) const
{
	return (m_path);
}