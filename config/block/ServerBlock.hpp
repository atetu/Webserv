/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:45 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/28 18:39:48 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP_
# define SERVERBLOCK_HPP_

#include <config/block/LocationBlock.hpp>
#include <util/Optional.hpp>
#include <util/unit/DataSize.hpp>
#include <iostream>
#include <vector>
#include <list>

class ServerBlock
{
	public:
		Optional<int> m_port;
		Optional<std::string> m_host;
	//	Optional<std::string> m_name; // a retirer  si j'ai bien compris
		Optional<std::list<std::string> > m_names;
		Optional<DataSize> m_maxBodySize;
		Optional<std::string> m_root;
		Optional<std::vector<LocationBlock> > m_locations;

	public:
		ServerBlock();
		ServerBlock(const ServerBlock &other);

		virtual
		~ServerBlock();

		ServerBlock&
		operator=(const ServerBlock &other);

		ServerBlock&
		port(int port);

		const Optional<int>
		port(void) const;

		ServerBlock&
		host(std::string host);

		ServerBlock&
		names(std::list<std::string> names);

		const Optional<std::list<std::string>>
		names(void) const;

		ServerBlock&
		maxBodySize(DataSize maxBodySize);

		ServerBlock&
		root(std::string root);

		ServerBlock&
		locations(std::vector<LocationBlock> locations);

	public:
		static const std::string DEFAULT_HOST;
};

#endif /* SERVERBLOCK_HPP_ */
