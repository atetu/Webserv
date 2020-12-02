/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeBlock.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:23:13 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:23:13 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/MimeBlock.hpp>

MimeBlock::MimeBlock() :
	m_includes(),
	m_define()
{
}

MimeBlock::~MimeBlock()
{
}

MimeBlock&
MimeBlock::includes(const std::vector<std::string> & includes)
{
	m_includes.set(includes);

	return (*this);
}

MimeBlock&
MimeBlock::define(const std::vector<Mime> & defines)
{
	m_define.set(defines);

	return (*this);
}