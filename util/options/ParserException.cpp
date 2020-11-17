/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserException.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 15:30:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/17 09:41:28 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserException.hpp"

ParserException::ParserException() :
		m_message()
{
}

ParserException::ParserException(std::string message):
		m_message(message)
{
}

ParserException::ParserException
(const ParserException &other):
		m_message(other.m_message)
{
}

ParserException::~ParserException() throw ()
{
}

ParserException&
ParserException::operator =(const ParserException &other)
{
	if (this != &other)
		m_message = other.m_message;
	return (*this);
}

const std::string&
ParserException::message() const
{
	return (m_message);
}

const char*
ParserException::what() const throw ()
{
	return (m_message.c_str());
}

