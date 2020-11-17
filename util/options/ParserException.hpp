/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserException.hpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 15:30:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/16 13:30:44 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEREXCEPTION_HPP_
# define PARSEREXCEPTION_HPP_

#include <string>
# include <exception>

class ParserException :
		public std::exception
{	
	private:
		std::string m_message;
	
	public:
		ParserException(void);
		ParserException(std::string message);
		ParserException(const ParserException &other);

		virtual
		~ParserException() throw ();

		ParserException&
		operator=(const ParserException &other);

		const std::string& message() const;

		const char* what() const throw ();
};

#endif /* ParserException_HPP_ */
