/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Option.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:29:18 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/13 11:14:10 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTION_HPP_
# define OPTION_HPP_

#include <iostream>
#include <exception>

class Option
{
	private:
		std::string m_short;
		std::string m_long;
		std::string m_description;
		bool m_hasValue;
		std::string m_valueName;
		std::string m_value;

	public:
		Option();
		Option(std::string shortArg, std::string longArg, std::string description, bool hasValue, std::string valueName, std::string value);
		Option(const Option &other);

		virtual	~Option();

		void addValue(void);
		void addConfigFile(std::string str);
		std::string getMLong();
		std::string getMShort();
		
		Option& operator=(const Option &other);

		class IllegalArgumentException : public std::exception
			{
				virtual const char* what() const throw();
			};	
};
#endif /* OPTION_HPP_ */
