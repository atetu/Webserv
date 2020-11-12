/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Option.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:29:18 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/12 18:12:52 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTION_HPP_
# define OPTION_HPP_

#include <iostream>
#include <exception>

class Option
{
	private:
		char m_short;
		std::string m_long;
		std::string m_description;
		bool m_hasValue;
		std::string m_valueName;
		std::string m_value;

	public:
		Option();
		Option(char _m_short, std::string _m_long, std::string _m_description,
			bool _m_hasValue, std::string _m_valueName, std::string _m_value);
		Option(const Option &other);

		virtual	~Option();

		void addValue(void);
		void addConfigFile(std::string str)
		
		Option& operator=(const Option &other);

		class IllegalArgumentException : public std::exception
			{
				virtual const char* what() const throw();
			};	
};
#endif /* OPTION_HPP_ */
