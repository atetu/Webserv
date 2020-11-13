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

Option::Option(std::string shortArg, std::string longArg, std::string description, bool hasValue, std::string valueName, std::string value) :
m_short(shortArg),
m_long(longArg),
m_description(description),
m_hasValue(hasValue),
m_valueName(valueName),
m_value(value)
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

std::string Option::getMLong(void)
{
	return (m_long);
}

std::string Option::getMShort(void)
{
	return (m_short);
}

const char *Option::IllegalArgumentException::what() const throw()
{
	return ("Error: Wrong arguments\n");
}