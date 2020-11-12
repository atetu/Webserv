/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Option.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:29:18 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 19:29:18 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <util/options/Option.hpp>
#include "Option.hpp"

Option::Option() :
m_short(0),
m_long(""),
m_description(""),
m_hasValue(false),
m_valueName(""),
m_value("")
{
}

Option::Option(char _m_short, std::string _m_long, std::string _m_description,
	bool _m_hasValue, std::string _m_valueName, std::string _m_value) :
m_short(_m_short),
m_long(_m_long),
m_description(_m_description),
m_hasValue(_m_hasValue),
m_valueName(_m_valueName),
m_value(_m_value)
{
}

Option::~Option()
{

}

void Option::addValue(void)
{
	if (!(m_hasValue))
		m_hasValue = true;
	else
		throw IllegalArgumentException();
}

void Option::addConfigFile(std::string str)
{
	if (m_valueName.compare("") == 0)
		m_valueName = str;
	else
		throw IllegalArgumentException();
}

const char *Option::IllegalArgumentException::what() const throw()
{
	return ("Error: Wrong arguments\n");
}