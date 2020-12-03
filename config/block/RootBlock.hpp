/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootBlock.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:13:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:13:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOTBLOCK_HPP_
# define ROOTBLOCK_HPP_

#include <config/block/ServerBlock.hpp>
#include <config/block/CGI.hpp>

class RootBlock
{
	private:
		std::list<ServerBlock*> m_serverBlockList;
		std::list<CGI*> m_CGIList;

	public:
		RootBlock();
		RootBlock(std::list<ServerBlock*>);
		RootBlock(const RootBlock &other);
		
		virtual
		~RootBlock();

		RootBlock&
		operator =(const RootBlock &other);

		RootBlock &server(const std::list<ServerBlock*> &serverBlockList);
		RootBlock &cgi(const std::list<CGI*> &CGIList);

		const std::list<ServerBlock*>
		server(void) const;

		const std::list<CGI*>
		cgi(void) const;
};

#endif /* ROOTBLOCK_HPP_ */
