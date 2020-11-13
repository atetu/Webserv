/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/13 12:09:51 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONPARSER_HPP_
# define OPTIONPARSER_HPP_

#include <iostream>
#include <string>
#include <list>
#include <exception>

//#include <util/options/Option.hpp>
#include "Option.hpp"
class OptionParser
{
	public:
		OptionParser();
		OptionParser(std::list<Option*> lst);
		OptionParser(const OptionParser &other);

		virtual
		~OptionParser();

		OptionParser&
		operator=(const OptionParser &other);

		void parse(int argc, char **argv);
		int whichOption(const char *str, const char *str2);
		int checkConfigFile(std::string str_obj, std::string str_obj2, int is_sec, std::list<Option*>::iterator it);
	
	class IllegalArgumentException : public std::exception
	{
		virtual const char* what() const throw();
	};	
	
	private:
		std::list<Option*> m_lst;
		// Option *m_helpOption;
		// Option *m_checkOption;
		// Option *m_configFileOption;
};

#endif /* OPTIONPARSER_HPP_ */
