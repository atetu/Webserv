/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:37:25 by alicetetu         #+#    #+#             */
/*   Updated: 2020/11/30 15:44:38 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/ErrorPageBlock.hpp>

ErrorPageBlock::ErrorPageBlock(void) :
		m_path(""),
		m_errorCode()
{
}

ErrorPageBlock::ErrorPageBlock(const ErrorPageBlock &other) :
		m_path(other.m_path),
		m_errorCode(other.m_errorCode)
{
}

ErrorPageBlock::~ErrorPageBlock()
{
}

ErrorPageBlock&
ErrorPageBlock::operator =(const ErrorPageBlock &other)
{
	if (this != &other)
	{
		m_path = other.m_path;
		m_errorCode = other.m_errorCode;
	}

	return (*this);
}

ErrorPageBlock&
ErrorPageBlock::path(std::string path)
{
	m_path = path;

	return (*this);
}

ErrorPageBlock&
ErrorPageBlock::code(int code)
{
	m_errorCode.set(code);

	return (*this);
}
