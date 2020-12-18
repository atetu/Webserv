/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootBlock.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:13:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:13:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOTBLOCK_HPP_
# define ROOTBLOCK_HPP_

#include <config/block/CGIBlock.hpp>
#include <config/block/MimeBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <util/Optional.hpp>
#include <list>
#include <string>

class RootBlock
{
	public:
		typedef std::list<const ServerBlock*> slist;
		typedef slist::const_iterator sciterator;

		typedef std::list<const CGIBlock*> clist;
		typedef clist::const_iterator cciterator;

	private:
		Optional<std::string> m_root;
		Optional<const MimeBlock*> m_mimeBlock;
		Optional<slist> m_serverBlocks;
		Optional<clist> m_cgiBlocks;

	public:
		RootBlock();
		RootBlock(const RootBlock &other);

		virtual
		~RootBlock();

		RootBlock&
		operator =(const RootBlock &other);

		RootBlock&
		root(const std::string &root);

		RootBlock&
		mimeBlock(const MimeBlock &mimeBlock);

		RootBlock&
		serverBlocks(const std::list<const ServerBlock*> &serverBlocks);

		RootBlock&
		cgiBlocks(const std::list<const CGIBlock*> &cgiBlocks);

		inline const Optional<std::string>&
		root(void) const
		{
			return (m_root);
		}

		inline const Optional<MimeBlock const*>&
		mimeBlock(void) const
		{
			return (m_mimeBlock);
		}

		inline const Optional<std::list<const ServerBlock*> >&
		serverBlocks(void) const
		{
			return (m_serverBlocks);
		}

		inline const Optional<std::list<const CGIBlock*> >&
		cgiBlocks(void) const
		{
			return (m_cgiBlocks);
		}

		inline const ServerBlock*
		findServerBlock(const std::string &clientHost) const
		{
			const ServerBlock *serverBlock;
			std::list<const ServerBlock*> serverBlockList = m_serverBlocks.get(); // ici ref
			std::list<const ServerBlock*>::iterator server_it = serverBlockList.begin();
			std::list<const ServerBlock*>::iterator server_ite = serverBlockList.end();
			int found = 0;

			while (server_it != server_ite)
			{
				if ((*server_it)->names().present())
				{
					std::list<std::string> serverNames = (*server_it)->names().get();
					std::list<std::string>::iterator serverNames_it = serverNames.begin();
					std::list<std::string>::iterator serverNames_ite = serverNames.end();

					while (serverNames_it != serverNames_ite)
					{
						if (serverNames_it->compare(clientHost) == 0)
						{
							serverBlock = *server_it;
							found = 1;
							break;
						}
						serverNames_it++;
					}

				}
				if (found)
					break;
				server_it++;
			}
			return (serverBlock); // what happens if does not exit?
		}

		inline bool
		hasCGI(const std::string &name) const
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

		inline const CGIBlock&
		getCGI(const std::string &name) const
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
};

#endif /* ROOTBLOCK_HPP_ */
