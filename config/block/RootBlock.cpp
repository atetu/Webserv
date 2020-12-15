/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootBlock.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:13:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:13:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/RootBlock.hpp>

RootBlock::RootBlock():
		m_serverBlockList(),
		m_CGIList()
{
}

RootBlock::RootBlock(std::list<ServerBlock*> serverBlockList) :
		m_serverBlockList(serverBlockList),
		m_CGIList()
{
}

RootBlock::RootBlock(const RootBlock &other) :
		m_serverBlockList(other.m_serverBlockList),
		m_CGIList(other.m_CGIList)
{
}

RootBlock&
RootBlock::operator =(const RootBlock &other)
{
	if (this != &other)
	{
		m_serverBlockList = other.m_serverBlockList;
		m_CGIList = other.m_CGIList;
	}
	return (*this);
}

RootBlock::~RootBlock()
{
	// TODO Auto-generated destructor stub
}

RootBlock &
RootBlock::server(const std::list<ServerBlock*> &serverBlockList)
{
	m_serverBlockList = serverBlockList;
	return (*this);
}

RootBlock &
RootBlock::cgi(const std::list<CGI*> &CGIList)
{
	m_CGIList = CGIList;
	return (*this);
}

const std::list<ServerBlock*>
RootBlock::server(void) const
{
	return (m_serverBlockList);
}

const std::list<CGI*>
RootBlock::cgi(void) const
{
	return (m_CGIList);
}
