/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/16 11:01:48 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONPARSER_HPP_
# define OPTIONPARSER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <exception>

//#include <util/options/Option.hpp>
#include "Option.hpp"
class OptionParser
{
	public:
		OptionParser();
		OptionParser(std::list<Option*> lst, std::map<char, std::string> *optionMap);
		OptionParser(const OptionParser &other);

		virtual
		~OptionParser();

		OptionParser&
		operator=(const OptionParser &other);

		void parse(int argc, char **argv);
		int whichOption(const char *str, const char *str2);
		int checkConfigFile(std::string str_obj, std::string str_obj2, int is_sec, std::list<Option*>::iterator it);
		int multipleShortOptions(std::string str_obj, std::string str_obj2, int is_sec, std::list<Option*>::iterator it);
		int checkShortOptions(std::list<Option*>::iterator it2, char charShortOpt, std::list<Option*>::iterator it_hasValue, std::list<Option*>::iterator it_end);
		int loopOption(std::list<Option*>::iterator it, std::string str_obj, std::list<Option*>::iterator it_hasValue, int i);
		int checkStart(std::list<Option*>::iterator it_hasValue, int i);
		void fillConfigFileWithSubstr(std::list<Option*>::iterator it_hasValue, std::string str_obj, int start_substr);
		int fillConfigFileWithSecondArg(std::list<Option*>::iterator it_hasValue, std::string str_obj2, int is_sec);

	class IllegalArgumentException : public std::exception
	{
		virtual const char* what() const throw();
	};	
	
	private:
		std::list<Option*> m_lst;
		std::map<char, std::string> *m_optionMap;
		// Option *m_helpOption;
		// Option *m_checkOption;
		// Option *m_configFileOption;
};

#endif /* OPTIONPARSER_HPP_ */
