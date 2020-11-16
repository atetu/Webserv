/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Option.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:29:18 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/14 12:44:48 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTION_HPP_
# define OPTION_HPP_

#include <iostream>
#include <exception>

class Option
{
	private:
		std::string m_shortStart;
		std::string m_longStart;
		char m_short;
		std::string m_long;
		std::string m_description;
		bool m_hasValue;
		std::string m_valueName;
		std::string m_value;

	public:
		Option();
		Option(std::string shortStart, std::string longStart, char shortArg, std::string longArg, std::string description, bool hasValue, std::string valueName, std::string value);
		Option(const Option &other);

		virtual	~Option();

		void addValue(void);
		void addConfigFile(std::string str);
		std::string getLongStart();
		std::string getShortStart();
		std::string getLongArg();
		char getShortArg();
		std::string getValue();
		std::string getDescription(void);
		
		bool hasValue(void);
		
		Option& operator=(const Option &other);

		class IllegalArgumentException : public std::exception
			{
				virtual const char* what() const throw();
			};	
};
#endif /* OPTION_HPP_ */
