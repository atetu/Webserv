/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 13:18:11 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/09 16:06:25 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP_
# define HTTPREQUEST_HPP_

#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPMethod.hpp>
#include <http/HTTPVersion.hpp>
#include <util/URL.hpp>

class HTTPRequest
{
	private:
		HTTPMethod m_method;
		URL m_url;
		HTTPVersion m_version;
		HTTPHeaderFields m_headerFields;
		const Configuration &m_configuration;
		const RootBlock &m_rootBlock;
		const ServerBlock &m_serverBlock;
		const LocationBlock &m_locationBlock;

	private:
		HTTPRequest(void);
		HTTPRequest(const HTTPRequest &other);

		HTTPRequest&
		operator =(const HTTPRequest &other);

	public:
		HTTPRequest(const HTTPMethod &method, const URL &url, const HTTPVersion &version, const HTTPHeaderFields &headerFields, const Configuration &configuration, const RootBlock &rootBlock, const ServerBlock &serverBlock, const LocationBlock &locationBlock);

		virtual
		~HTTPRequest(void);

		inline const HTTPMethod&
		method()
		{
			return (m_method);
		}

		inline const URL&
		url()
		{
			return (m_url);
		}

		inline const HTTPVersion&
		version()
		{
			return (m_version);
		}

		inline const HTTPHeaderFields&
		headers()
		{
			return (m_headerFields);
		}

		inline const Configuration&
		configuration()
		{
			return (m_configuration);
		}

		inline const ServerBlock&
		server()
		{
			return (m_serverBlock);
		}

		inline const LocationBlock&
		location()
		{
			return (m_locationBlock);
		}

		inline std::string
		root(void) const
		{
			if (m_locationBlock.root().present())
				return (m_locationBlock.root().get());

			if (m_serverBlock.root().present())
				return (m_serverBlock.root().get());

			if (m_rootBlock.root().present())
				return (m_rootBlock.root().get());

			return ("./");
		}
};

#endif /* HTTPREQUEST_HPP_ */
