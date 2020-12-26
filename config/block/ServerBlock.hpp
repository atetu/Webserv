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
#include <list>
#include <string>

class ServerBlock
{
	public:
		static const std::string DEFAULT_HOST;
		static const short DEFAULT_PORT;

	private:
		Optional<int> m_port;
		Optional<std::string> m_host;
		Optional<bool> m_isDefault;
		Optional<std::list<std::string> > m_names;
		Optional<DataSize> m_maxBodySize;
		Optional<std::string> m_root;
		Optional<std::list<LocationBlock const*> > m_locations;
		Optional<CustomErrorMap> m_errors;
		Optional<std::list<std::string> > m_methods;
		Optional<bool> m_listing;
		Optional<std::list<std::string> > m_indexFiles;

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
		host(const std::string &host);

		ServerBlock&
		isDefault(bool isDefault);

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

		ServerBlock&
		methods(const std::list<std::string> &methods);

		ServerBlock&
		listing(bool listing);

		ServerBlock&
		index(const std::list<std::string> &files);

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

		inline const Optional<bool>&
		isDefault(void) const
		{
			return (m_isDefault);
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

		inline const Optional<std::list<std::string> >&
		methods(void) const
		{
			return (m_methods);
		}

		inline const Optional<bool>&
		listing(void) const
		{
			return (m_listing);
		}

		inline const Optional<std::list<std::string> >&
		index(void) const
		{
			return (m_indexFiles);
		}

		bool
		hasName(const std::string &name) const;

		bool
		hasMethod(const std::string &name) const;
};

#endif /* SERVERBLOCK_HPP_ */
