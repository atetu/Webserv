/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/12 18:18:26 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <util/options/OptionParser.hpp>

#include "OptionParser.hpp"

OptionParser::OptionParser() :
m_helpOption(NULL),
m_checkOption(NULL),
m_configFileOption(NULL)
{}

OptionParser::OptionParser(Option *_helpOption, Option *_checkOption,
	Option *_configFileOption) :
m_helpOption(_helpOption),
m_checkOption(_checkOption),
m_configFileOption(_configFileOption)
{}

OptionParser::~OptionParser()
{
	
}

void OptionParser::parse(int argc, char **argv)
{
	if (argc == 1)
		throw IllegalArgumentException();
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-' && argv[i][1] && argv[i][1] == '-')
				longOption(argv[i]);
			if (argv[i][0] == '-')
				shortOption(argv[i]);
			else
				configFileOption(argv[i]);
		}
 	}
}

void OptionParser::longOption(const char *str)
{
	std::string str_obj = str;
	if (str_obj.compare("--help") == 0)
		m_helpOption->addValue();
	else if (str_obj.compare("--check") == 0)
		m_checkOption->addValue();
	else if (str_obj.compare("--config-file") == 0)
		m_configFileOption->addValue();
	else /// a voir si l'on admet des fichiers qui commencent par --
		m_configFileOption->addConfigFile(str_obj);

}

void OptionParser::shortOption(const char *str)
{
	std::string str_obj = str;
	if (str_obj.compare("-h") == 0)
		m_helpOption->addValue();
	else if (str_obj.compare("-c") == 0)
		m_checkOption->addValue();
	else if (str_obj.compare("-f") == 0)
		m_configFileOption->addValue();
	else /// a voir si l'on admet des fichiers qui commencent par --
		m_configFileOption->addConfigFile(str_obj);

}

void OptionParser::configFileOption(const char *str)
{
	std::string str_obj = str;
	m_configFileOption->addConfigFile(str_obj);
}

const char *OptionParser::IllegalArgumentException::what() const throw()
{
	return ("Error: Wrong arguments\n");
}