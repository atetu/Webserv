/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserIllegalArgumentException.hpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 15:30:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/16 13:30:44 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERILLEGALARGUMENTEXCEPTION_HPP_
# define PARSERILLEGALARGUMENTEXCEPTION_HPP_

#include <string>
# include <exception>

class ParserIllegalArgumentException :
		public std::exception
{	
	private:
		std::string m_message;
	
	public:
		ParserIllegalArgumentException(void);
		ParserIllegalArgumentException(std::string message);
		ParserIllegalArgumentException(const ParserIllegalArgumentException &other);

		virtual
		~ParserIllegalArgumentException() throw ();

		ParserIllegalArgumentException&
		operator=(const ParserIllegalArgumentException &other);

		const std::string& message() const;

		const char* what() const throw ();
};

#endif /* PARSERILLEGALARGUMENTEXCEPTION_HPP_ */
