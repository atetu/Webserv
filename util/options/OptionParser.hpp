/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/16 17:06:25 by alicetetu        ###   ########.fr       */
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
		OptionParser(std::list<Option*> lst, std::map<char,
			std::string> *optionMap);
		OptionParser(const OptionParser &other);

		virtual
		~OptionParser();

		OptionParser&
		operator=(const OptionParser &other);

		void parse(int argc, char **argv);
		int whichOption(const char *str, const char *str2);
		int checkConfigFile(const std::string &str_obj2, int is_sec,
			const std::list<Option*>::iterator &it);
		int multipleShortOptions(const std::string &str_obj, const std::string &str_obj2,
			int is_sec, const std::list<Option*>::iterator &it);
		int checkShortOptions(const std::list<Option*>::iterator &it2,
			char charShortOpt, std::list<Option*>::iterator &it_hasValue,
			const std::list<Option*>::iterator &it_end);
		int loopOption(const std::list<Option*>::iterator &it, const std::string &str_obj,
			std::list<Option*>::iterator &it_hasValue, int i);
		int fillConfigFileWithSubstr(const std::list<Option*>::iterator &it_hasValue,
			const std::string &str_obj, int start_substr);
		int fillConfigFileWithSecondArg(const std::list<Option*>::iterator
			&it_hasValue, const std::string &str_obj2, int is_sec);
		int loopLongAndShortOption(std::string &str_obj, const std::string &str_obj2, int is_sec);
		void fillConfigFileInCaseOfNoOption(const std::string &str_obj);
		
	private:
		std::list<Option*> m_lst;
		std::map<char, std::string> *m_optionMap;
};

#endif /* OPTIONPARSER_HPP_ */