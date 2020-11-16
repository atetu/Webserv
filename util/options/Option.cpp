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
m_shortStart(""),
m_longStart(""),
m_short(0),
m_long(""),
m_description(""),
m_hasValue(false),
m_valueName(""),
m_value("")
{
}

Option::Option(std::string shortStart, std::string longStart, char shortArg, std::string longArg, std::string description, bool hasValue, std::string valueName, std::string value) :
m_shortStart(shortStart),
m_longStart(longStart),
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
	{
		m_value = str;
		m_hasValue = true;
	}
	else
		throw IllegalArgumentException();
}

std::string Option::getLongStart(void)
{
	return (m_longStart);
}

std::string Option::getShortStart(void)
{
	return (m_shortStart);
}

std::string Option::getLongArg(void)
{
	return (m_long);
}

char Option::getShortArg(void)
{
	return (m_short);
}

std::string Option::getValue(void)
{
	return (m_value);
}

bool Option::hasValue(void)
{
	return (m_hasValue);
}

std::string Option::getDescription(void)
{
	return (m_description);
}

const char *Option::IllegalArgumentException::what() const throw()
{
	return ("Error: Wrong arguments\n");
}