/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/12 18:17:03 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONPARSER_HPP_
# define OPTIONPARSER_HPP_

#include <iostream>
#include <string>
#include <exception>

//#include <util/options/Option.hpp>
#include "Option.hpp"
class OptionParser
{
	public:
		OptionParser();
		OptionParser(Option *_helpOption, Option *_checkOption,
			Option *_configFileOption);
		OptionParser(const OptionParser &other);

		virtual
		~OptionParser();

		OptionParser&
		operator=(const OptionParser &other);

		void parse(int argc, char **argv);
		void longOption(const char *str);
		void shortOption(const char *str);
		void configFileOption(const char *str);
	
	class IllegalArgumentException : public std::exception
	{
		virtual const char* what() const throw();
	};	
	
	private:
		Option *m_helpOption;
		Option *m_checkOption;
		Option *m_configFileOption;
};

#endif /* OPTIONPARSER_HPP_ */
