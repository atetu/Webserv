/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:45 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 11:17:58 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP_
# define SERVERBLOCK_HPP_

#include <config/block/container/CustomErrorMap.hpp>
#include <config/block/LocationBlock.hpp>
#include <util/Optional.hpp>
#include <util/unit/DataSize.hpp>
#include <http/HTTPFindLocation.hpp>
#include <list>
#include <map>
#include <string>

class ServerBlock
{
	public:
		static const std::string DEFAULT_HOST;
		static const short DEFAULT_PORT;

	private:
		Optional<int> m_port;
		Optional<std::string> m_host;
		Optional<std::list<std::string> > m_names;
		Optional<DataSize> m_maxBodySize;
		Optional<std::string> m_root;
		Optional<std::list<LocationBlock const*> > m_locations;
		Optional<CustomErrorMap> m_errors;

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
		maxBodySize(const DataSize &maxBodySize);

		ServerBlock&
		root(const std::string &root);

		ServerBlock&
		locations(const std::list<LocationBlock const*> &locations);

		ServerBlock&
		errors(const CustomErrorMap &errors);

		inline const Optional<int>
		port(void) const
		{
			return (m_port);
		}

		inline const Optional<std::string>&
		host(void) const
		{
			return (m_host);
		}

		inline const Optional<std::list<std::string> >
		names(void) const
		{
			return (m_names);
		}

		inline const Optional<DataSize>
		maxBodySize(void) const
		{
			return (m_maxBodySize);
		}

		inline const Optional<std::string>&
		root(void) const
		{
			return (m_root);
		}

		inline const Optional<std::list<LocationBlock const*> >&
		locations(void) const
		{
			return (m_locations);
		}

		inline const Optional<CustomErrorMap>&
		errors(void) const
		{
			return (m_errors);
		}

		// inline const LocationBlock*
		// ServerBlock::findLocation(const std::string &path) const
		// {
		// 	HTTPFindLocation findLocation(path, m_locations.get());
		// 	return (findLocation.parse().location().get());
		// }
};

#endif /* SERVERBLOCK_HPP_ */
