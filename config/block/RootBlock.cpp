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

RootBlock::RootBlock() :
		m_root(),
		m_mimeBlock(),
		m_serverBlocks(),
		m_cgiBlocks()
{
}

RootBlock::RootBlock(const RootBlock &other) :
		m_root(other.m_root),
		m_mimeBlock(other.m_mimeBlock),
		m_serverBlocks(other.m_serverBlocks),
		m_cgiBlocks(other.m_cgiBlocks)
{
}

RootBlock::~RootBlock()
{
}

RootBlock&
RootBlock::operator =(const RootBlock &other)
{
	if (this != &other)
	{
		m_root = other.m_root;
		m_mimeBlock = other.m_mimeBlock;
		m_serverBlocks = other.m_serverBlocks;
		m_cgiBlocks = other.m_cgiBlocks;
	}

	return (*this);
}

RootBlock&
RootBlock::root(const std::string &root)
{
	m_root.set(root);

	return (*this);
}

RootBlock&
RootBlock::mimeBlock(const MimeBlock &mimeBlock)
{
	m_mimeBlock.set(&mimeBlock);

	return (*this);
}

RootBlock&
RootBlock::serverBlocks(const std::list<const ServerBlock*> &serverBlocks)
{
	m_serverBlocks.set(serverBlocks);

	return (*this);
}

RootBlock&
RootBlock::cgiBlocks(const std::list<const CGIBlock*> &cgiBlocks)
{
	m_cgiBlocks.set(cgiBlocks);

	return (*this);
}
