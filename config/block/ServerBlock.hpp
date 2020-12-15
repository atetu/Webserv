/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:45 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/15 12:30:28 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP_
# define SERVERBLOCK_HPP_

#include <config/block/LocationBlock.hpp>
#include <util/Optional.hpp>
#include <util/unit/DataSize.hpp>
#include <list>
#include <map>
#include <string>
#include <vector>

class ServerBlock
{
	private:
		Optional<int> m_port;
		Optional<std::string> m_host;
		Optional<std::list<std::string> > m_names;
		Optional<DataSize> m_maxBodySize;
		Optional<std::string> m_root;
		Optional<std::vector<LocationBlock*> > m_locations;
		Optional<std::map<int, std::string> > m_errors;

	public:
		ServerBlock();
		ServerBlock(const ServerBlock &other);

		virtual
		~ServerBlock();

		ServerBlock&
		operator=(const ServerBlock &other);

		ServerBlock&
		port(int port);

		ServerBlock&
		host(std::string host);

		ServerBlock&
		names(const std::list<std::string> &names);

		ServerBlock&
		maxBodySize(DataSize maxBodySize);

		ServerBlock&
		root(const std::string &root);

		ServerBlock&
		locations(std::vector<LocationBlock*> locations);

		ServerBlock&
		errors(std::map<int, std::string> errors);

		const Optional<int>
		port(void) const
		{
			return (m_port);
		}

		const Optional<std::string>&
		host(void) const
		{
			return (m_host);
		}

		const Optional<std::list<std::string> >
		names(void) const
		{
			return (m_names);
		}

		inline const Optional<std::string>&
		root(void) const
		{
			return (m_root);
		}

		const Optional<std::vector<LocationBlock*> >&
		locations(void) const
		{
			return (m_locations);
		}

	public:
		static const std::string DEFAULT_HOST;
};

#endif /* SERVERBLOCK_HPP_ */
