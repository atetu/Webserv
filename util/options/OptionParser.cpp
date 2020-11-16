/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/16 11:52:23 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <util/options/OptionParser.hpp>

#include "OptionParser.hpp"

OptionParser::OptionParser() :
m_lst(0) //est ce qu'une liste peut etre nulle?
{}

OptionParser::OptionParser(std::list<Option*> lst, std::map<char, std::string> *optionMap) :
m_lst(lst),
m_optionMap(optionMap)
{}

OptionParser::~OptionParser()
{
	
}

void OptionParser::parse(int argc, char **argv)
{
	int config_file;
	char *sec;

	if (argc == 1)
		throw IllegalArgumentException();
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
	std::string str_obj2;
	std::list<Option*>::iterator itHasValue;
	
	is_sec = 0;
	std::string str_obj = str;
	if (str2)
	{
		is_sec = 1;
		str_obj2 = str2;
	}
	if (((((*m_lst.begin())->getLongStart()).compare(str_obj.substr(0, 2))) == 0)
		|| ((((*m_lst.begin())->getShortStart()).compare(str_obj.substr(0, 1))) == 0))
	{
			for (std::list<Option*>::iterator it = m_lst.begin(); it!= m_lst.end(); ++it)
			{				
				if (((((*it)->getLongStart()).compare(str_obj.substr(0, 2))) == 0)
					&& ((((*it)->getLongArg()).compare(str_obj.substr(2, std::string::npos))) == 0))
					return (checkConfigFile(str_obj2, is_sec, it));
				else if (((((*it)->getLongStart()).compare(str_obj.substr(0, 2))) == 0)
					&& (((((*it)->getLongArg()) + "=").compare(str_obj.substr(2, 12))) == 0))
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
				if ((*it)->hasValue()) //pas sure que ce soit necessaire
					itHasValue = it;
			}
	}
	else
	{
		(*m_optionMap)['f'] = str_obj;
		(*itHasValue)->addConfigFile(str_obj); // pas sure que ce soit necessaire
	}
	return(0);
}

int OptionParser::checkConfigFile(std::string str_obj2, int is_sec, std::list<Option*>::iterator it)
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
			throw IllegalArgumentException();
	}
	else
		(*m_optionMap)[(*it)->getShortArg()] = "";
			
	return (0);
}

int OptionParser::multipleShortOptions(std::string str_obj, std::string str_obj2, int is_sec, std::list<Option*>::iterator it)
{
	std::list<Option*>::iterator it_hasValue;
	int is_found;
	int start_substr;
		
	it_hasValue = m_lst.end();
	if ((*it)->hasValue())
		it_hasValue = it;
	(*m_optionMap)[(*it)->getShortArg()] = "";
	for (int i = 2; i < str_obj.size(); i++)
	{
		if (!(is_found = loopOption(it, str_obj, it_hasValue, i)))
		{
			start_substr = checkStart(it_hasValue, i);
			break ;
		}
	}
	if (it_hasValue != m_lst.end())
	{
		if (!is_found)
			fillConfigFileWithSubstr(it_hasValue, str_obj, start_substr);
		else
			return (fillConfigFileWithSecondArg(it_hasValue, str_obj2, is_sec));
	}
	return (0);
}


int OptionParser::checkShortOptions(std::list<Option*>::iterator it2, char charShortOpt, std::list<Option*>::iterator it_hasValue, std::list<Option*>::iterator it_end)
{
	if ((*it2)->getShortArg() == charShortOpt)
	{	
		(*m_optionMap)[(*it2)->getShortArg()] = "";
		if ((*it2)->hasValue())
		{
			if (it_hasValue == it_end)
				it_hasValue = it2;
			else
				throw IllegalArgumentException();
		}
		return (1);
	}
	return (0);
}

int OptionParser::loopOption(std::list<Option*>::iterator it, std::string str_obj, std::list<Option*>::iterator it_hasValue, int i)
{
	int is_found;
	std::list<Option*>::iterator it_copy;

	is_found = 0;
	it_copy = it;
	++it_copy;
	for (std::list<Option*>::iterator it2 = m_lst.begin(); it2!= it; ++it2)
	{
		if((is_found = checkShortOptions(it2, str_obj.c_str()[i], it_hasValue, m_lst.end())))
			break;
	}
	if (!is_found)
	{
		for (std::list<Option*>::iterator it2 = it_copy; it2!= m_lst.end(); ++it2)
		{
			if((is_found = checkShortOptions(it2,
				str_obj.c_str()[i], it_hasValue, m_lst.end())))
				break;
		}
	}
	return (is_found);
}

int OptionParser::checkStart(std::list<Option*>::iterator it_hasValue, int i)
{
	if (it_hasValue != m_lst.end())
		return (i);
	else
		throw IllegalArgumentException();
}

void OptionParser::fillConfigFileWithSubstr(std::list<Option*>::iterator it_hasValue, std::string str_obj, int start_substr)
{
	(*m_optionMap)[(*it_hasValue)->getShortArg()] = str_obj.substr(start_substr, std::string::npos);
	(*it_hasValue)->addConfigFile(str_obj.substr(start_substr, std::string::npos));
}

int OptionParser::fillConfigFileWithSecondArg(std::list<Option*>::iterator it_hasValue, std::string str_obj2, int is_sec)
{
	if (is_sec)
	{
		(*m_optionMap)[(*it_hasValue)->getShortArg()] = str_obj2;
		(*it_hasValue)->addConfigFile(str_obj2);
		return (1);
	}
	else
		throw IllegalArgumentException();
	return (0);
}
const char *OptionParser::IllegalArgumentException::what() const throw()
{
	return ("Error: Wrong arguments\n");
}