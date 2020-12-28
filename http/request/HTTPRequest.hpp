/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 13:18:11 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 17:02:15 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP_
# define HTTPREQUEST_HPP_

#include <config/block/LocationBlock.hpp>
#include <http/enums/HTTPVersion.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <util/Optional.hpp>
#include <util/URL.hpp>
#include <string>

class Authorization;

class Configuration;
class RootBlock;
class ServerBlock;

class HTTPRequest
{
	private:
		const HTTPMethod &m_method;
		URL m_url;
		HTTPVersion m_version;
		HTTPHeaderFields m_headerFields;
		const std::string m_body;
		const Configuration &m_configuration;
		const RootBlock &m_rootBlock;
		const ServerBlock &m_serverBlock;
		const Optional<LocationBlock const*> &m_locationBlock;
		Authorization *m_authorization;

	private:
		HTTPRequest(void);
		HTTPRequest(const HTTPRequest &other);

		HTTPRequest&
		operator =(const HTTPRequest &other);

	public:
		HTTPRequest(const HTTPMethod &method, const URL &url, const HTTPVersion &version, const HTTPHeaderFields &headerFields, const std::string body, const Configuration &configuration, const RootBlock &rootBlock, const ServerBlock &serverBlock, const Optional<LocationBlock const*> &locationBlock);

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

		inline const std::string&
		body()
		{
			return (m_body);
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

		inline const Optional<LocationBlock const*>&
		location()
		{
			return (m_locationBlock);
		}

		std::string
		root(void) const;

		bool
		needAuth() const;

		const AuthBlock*
		auth() const;

		std::string
		getLocation();

};

#endif /* HTTPREQUEST_HPP_ */
