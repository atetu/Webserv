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
#include "ParserException.hpp"

Option::Option() :
m_shortStart(""),
m_longStart(""),
m_short(0),
m_long(""),
m_description(""),
m_hasValue(false),
m_valueName(""), // pas sure que cette variable soit desormais necessaire
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
m_valueName(valueName), // pas sure de cette variable
m_value(value)
{
}

Option::~Option()
{
}

Option::Option(const Option &other):
m_shortStart(other.m_shortStart),
m_longStart(other.m_longStart),
m_short(other.m_short),
m_long(other.m_long),
m_description(other.m_description),
m_hasValue(other.m_hasValue),
m_valueName(other.m_valueName), // pas sure de cette variable
m_value(other.m_value)
{	
}

Option &
Option::operator=(const Option &other)
{
	if (this != &other)
	{
		m_shortStart = other.m_shortStart;
		m_longStart = other.m_longStart;
		m_short = other.m_short;
		m_long = other.m_long;
		m_description = other.m_description;
		m_hasValue = other.m_hasValue;
		m_valueName = other.m_valueName;// pas sure de cette variable
		m_value = other.m_value;
	}

	return (*this);
}


void Option::addConfigFile(std::string str)
{
	if (m_value == "")
	{
		m_value = str;
		m_hasValue = true;
	}
	else
		throw ParserException("Wrong arguments: Only one config file needed");
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
