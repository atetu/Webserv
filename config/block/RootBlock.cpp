/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:13:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:13:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/RootBlock.hpp>
#include <util/helper/DeleteHelper.hpp>

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
//	std::cout << (char)*((char*)NULL) << std::endl;

	DeleteHelper::pointers<ServerBlock>(m_serverBlocks);
	DeleteHelper::pointers<CGIBlock>(m_cgiBlocks);
	DeleteHelper::pointer<MimeBlock>(m_mimeBlock);
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

bool
RootBlock::hasCGI(const std::string &name) const
{
	typedef std::list<const CGIBlock*> list;

	if (m_cgiBlocks.present())
	{
		const list &cgiBlocks = m_cgiBlocks.get();

		for (list::const_iterator it = cgiBlocks.begin(); it != cgiBlocks.end(); it++)
		{
			if ((*it)->name() == name)
				return (true);
		}
	}

	return (false);
}

const CGIBlock&
RootBlock::getCGI(const std::string &name) const
{
	typedef std::list<const CGIBlock*> list;

	const list &cgiBlocks = m_cgiBlocks.get();

	for (list::const_iterator it = cgiBlocks.begin(); it != cgiBlocks.end(); it++)
	{
		const CGIBlock &cgiBlock = *(*it);

		if (cgiBlock.name() == name)
			return (cgiBlock);
	}

	throw NullPointerException("No CGI found with name: " + name);
}
