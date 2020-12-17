/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:23:13 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:23:13 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/MimeBlock.hpp>
#include <util/helper/DeleteHelper.hpp>

MimeBlock::MimeBlock() :
		m_includes(),
		m_defines()
{
}

MimeBlock::MimeBlock(const MimeBlock &other) :
		m_includes(other.m_includes),
		m_defines(other.m_defines)
{
}

MimeBlock::~MimeBlock()
{
	DeleteHelper::deletePointerList<Mime>(m_defines);
}

MimeBlock&
MimeBlock::operator =(const MimeBlock &other)
{
	if (this != &other)
	{
		m_includes = other.m_includes;
		m_defines = other.m_defines;
	}

	return (*this);
}

MimeBlock&
MimeBlock::includes(const std::list<std::string> &includes)
{
	m_includes.set(includes);

	return (*this);
}

MimeBlock&
MimeBlock::defines(const std::list<Mime const*> &defines)
{
	m_defines.set(defines);

	return (*this);
}
