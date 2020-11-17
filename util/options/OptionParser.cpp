/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/17 09:38:57 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <util/options/OptionParser.hpp>

#include "OptionParser.hpp"
#include "ParserException.hpp"

OptionParser::OptionParser() :
m_lst(0), //est ce qu'une liste peut etre nulle?
m_optionMap(0)
{
}

OptionParser::OptionParser(std::list<Option*> lst, std::map<char, std::string> *optionMap) :
m_lst(lst),
m_optionMap(optionMap)
{
}

OptionParser::~OptionParser()
{
}

OptionParser::OptionParser(const OptionParser &other):
m_lst(other.m_lst),
m_optionMap(other.m_optionMap)
{	
}

OptionParser &
OptionParser::operator=(const OptionParser &other)
{
	if (this != &other)
	{
		m_lst = other.m_lst;
		m_optionMap = other.m_optionMap;
	}

	return (*this);
}

void OptionParser::parse(int argc, char **argv)
{
	int config_file;
	char *sec;

	if (argc == 1)
		throw ParserException("Not enough arguments\n");
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (i != argc - 1)
				sec = argv[i + 1];
			else
				sec = NULL;
			if ((config_file = whichOption(argv[i], sec)))
				i++;
		}
	}
}

int OptionParser::whichOption(const char *str, const char *str2)
{
	int is_sec;
	int ret;
	std::string str_obj2;
		
	is_sec = 0;
	std::string str_obj = str;
	if (str2)
	{
		is_sec = 1;
		str_obj2 = str2;
	}
	if (((((*m_lst.begin())->getLongStart()).compare(str_obj.substr(0, 2))) == 0)
		|| ((((*m_lst.begin())->getShortStart()).compare(str_obj.substr(0, 1)))
		== 0))
	{
		if ((ret = loopLongAndShortOption(str_obj, str_obj2, is_sec)) != -1)
			return (ret);
	}
	else
		fillConfigFileInCaseOfNoOption(str_obj);
	return(0);
}

int OptionParser::loopLongAndShortOption(std::string &str_obj,
	const std::string &str_obj2, int is_sec)
{
	for (std::list<Option*>::iterator it = m_lst.begin();
		it!= m_lst.end(); ++it)
	{				
		if (((((*it)->getLongStart()).compare(str_obj.substr(0, 2))) == 0)
			&& ((((*it)->getLongArg()).compare(str_obj.substr(2,
			std::string::npos))) == 0))
			return (checkConfigFile(str_obj2, is_sec, it));
		else if (((((*it)->getLongStart()).compare(str_obj.substr(0, 2)))
			== 0) && (((((*it)->getLongArg()) + "=").compare
			(str_obj.substr(2, 12))) == 0))
		{
			checkConfigFile(str_obj.substr(14, std::string::npos), 1, it);
			return(0);
		}
		else if (((((*it)->getShortStart()).compare(str_obj.substr(0, 1))) == 0)
			&& ((((*it)->getShortArg()) == str_obj.c_str()[1]))
			&& (str_obj.c_str()[2]) == 0)
			return (checkConfigFile(str_obj2, is_sec, it));
		else if (((((*it)->getShortStart()).compare(str_obj.substr(0, 1))) == 0)
			&& ((((*it)->getShortArg()) == str_obj.c_str()[1]))
			&& (str_obj.c_str()[2]) != 0)
			return (multipleShortOptions(str_obj, str_obj2, is_sec, it));
	}
	return (-1);
}

void OptionParser::fillConfigFileInCaseOfNoOption(const std::string &str_obj)
{
	std::list<Option*>::iterator itHasValue;
	
	for (std::list<Option*>::iterator it = m_lst.begin();
		it!= m_lst.end(); ++it)
	{
		if ((*it)->hasValue())
			itHasValue = it;
	}
	(*m_optionMap)['f'] = str_obj;
	(*itHasValue)->addConfigFile(str_obj); // pas sure que ce soit necessaire
}

int OptionParser::checkConfigFile(const std::string &str_obj2, int is_sec,
const std::list<Option*>::iterator &it)
{
	if ((*it)->hasValue())
	{
		if(is_sec)
		{
			(*m_optionMap)[(*it)->getShortArg()] = str_obj2;
			(*it)->addConfigFile(str_obj2); // pas sure que ce soit utile
			return (1);
		}
		else
			throw ParserException
			("Config file required when using the config-file option");
	}
	else
		(*m_optionMap)[(*it)->getShortArg()] = "";		
	return (0);
}

int OptionParser::multipleShortOptions(const std::string &str_obj,
	const std::string &str_obj2, int is_sec, const std::list<Option*>::iterator &it)
{
	std::list<Option*>::iterator it_hasValue;
	int is_found;
			
	it_hasValue = m_lst.end();
	if ((*it)->hasValue())
		return (fillConfigFileWithSubstr(it, str_obj, 1));
	(*m_optionMap)[(*it)->getShortArg()] = "";
	for (int i = 2; i < str_obj.size(); i++)
	{
		if ((is_found = loopOption(it, str_obj, it_hasValue, i)))
		{
			if (it_hasValue != m_lst.end())
			{
				if (i + 1 != str_obj.size())
					return (fillConfigFileWithSubstr(it_hasValue, str_obj,
					i + 1));
				else
					return (fillConfigFileWithSecondArg(it_hasValue,
						str_obj2, is_sec));
			}
		}
		else
			throw ParserException
			("Error: \"" + str_obj + "\" not recognized as an argument\n");
	}
	return (0);
}


int OptionParser::checkShortOptions(const std::list<Option*>::iterator &it2,
char charShortOpt, std::list<Option*>::iterator &it_hasValue,
const std::list<Option*>::iterator &it_end)
{
	if ((*it2)->getShortArg() == charShortOpt)
	{	
		(*m_optionMap)[(*it2)->getShortArg()] = "";
		if ((*it2)->hasValue())
		{
			if (it_hasValue == it_end)
				it_hasValue = it2;
			else
				throw ParserException
				("Error: each option should be used only once\n");
		}
		return (1);
	}
	return (0);
}

int OptionParser::loopOption(const std::list<Option*>::iterator &it,
const std::string &str_obj, std::list<Option*>::iterator &it_hasValue, int i)
{
	int is_found;
	std::list<Option*>::iterator it_copy;

	is_found = 0;
	it_copy = it;
	++it_copy;
	for (std::list<Option*>::iterator it2 = m_lst.begin(); it2!= m_lst.end();
	++it2)
	{
		if((is_found = checkShortOptions(it2, str_obj.c_str()[i],
			it_hasValue, m_lst.end())))
			break;
	}
	return (is_found);
}

int OptionParser::fillConfigFileWithSubstr(const std::list<Option*>::iterator
&it_hasValue, const std::string &str_obj, int start_substr)
{
	(*m_optionMap)[(*it_hasValue)->getShortArg()] =
		str_obj.substr(start_substr, std::string::npos);
	(*it_hasValue)->addConfigFile(str_obj.substr(start_substr,
		std::string::npos));
	return (0);
}

int OptionParser::fillConfigFileWithSecondArg(const std::list<Option*>::iterator
&it_hasValue, const std::string &str_obj2, int is_sec)
{
	if (is_sec)
	{
		(*m_optionMap)[(*it_hasValue)->getShortArg()] = str_obj2;
		(*it_hasValue)->addConfigFile(str_obj2);
		return (1);
	}
	else
		throw ParserException
			("Config file required when using the config-file option\n");
	return (0);
}
