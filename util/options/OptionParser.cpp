/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/13 15:32:57 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <util/options/OptionParser.hpp>

#include "OptionParser.hpp"

OptionParser::OptionParser() :
m_lst(0) //est ce qu'une liste peut etre nulle?
{}

OptionParser::OptionParser(std::list<Option*> lst) :
m_lst(lst)
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
	
	is_sec = 0;
	std::string str_obj = str;
	if (str2)
	{
		is_sec = 1;
		str_obj2 = str2;
	}
	for (std::list<Option*>::iterator it = m_lst.begin(); it!= m_lst.end(); ++it)
	{
		if ((((*it)->getMLong()).compare(str_obj)) == 0)
		{
			(*it)->addValue();
			return (checkConfigFile(str_obj, str_obj2, is_sec, it));
		}	
		else if ((((*it)->getMShort()).compare(str_obj)) == 0)
		{
			(*it)->addValue();
			return (checkConfigFile(str_obj, str_obj2, is_sec, it));
		}
		else if ((((*it)->getMShort()).compare("-f")) == 0)
			(*it)->addConfigFile(str_obj);
	}
	return (0);
}

int OptionParser::checkConfigFile(std::string str_obj, std::string str_obj2, int is_sec, std::list<Option*>::iterator it)
{
	if ((str_obj.compare("--config-file") == 0) || (str_obj.compare("-f") == 0))
	{
		if (is_sec)
		{
			(*it)->addConfigFile(str_obj2);
			return (1);
		}
		else
			throw IllegalArgumentException();				
	}
	return (0);
}

const char *OptionParser::IllegalArgumentException::what() const throw()
{
	return ("Error: Wrong arguments\n");
}