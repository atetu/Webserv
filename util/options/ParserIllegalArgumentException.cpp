/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserIllegalArgumentException.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 15:30:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/16 16:26:25 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserIllegalArgumentException.hpp"

ParserIllegalArgumentException::ParserIllegalArgumentException() :
		m_message()
{
}

ParserIllegalArgumentException::ParserIllegalArgumentException(std::string message):
		m_message(message)
{
}

ParserIllegalArgumentException::ParserIllegalArgumentException
(const ParserIllegalArgumentException &other):
		m_message(other.m_message)
{
}

ParserIllegalArgumentException::~ParserIllegalArgumentException() throw ()
{
}

ParserIllegalArgumentException&
ParserIllegalArgumentException::operator =(const ParserIllegalArgumentException &other)
{
	if (this != &other)
		m_message = other.m_message;
	return (*this);
}

const std::string&
ParserIllegalArgumentException::message() const
{
	return (m_message);
}

const char*
ParserIllegalArgumentException::what() const throw ()
{
	return (m_message.c_str());
}

