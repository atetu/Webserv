/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GenericHTTPRequest.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 13:18:11 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 17:02:15 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERICHTTPREQUEST_HPP_
# define GENERICHTTPREQUEST_HPP_

#include <config/block/LocationBlock.hpp>
#include <http/enums/HTTPVersion.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/request/HTTPRequest.hpp>
#include <util/Optional.hpp>
#include <util/URL.hpp>
#include <string>

class Configuration;
class RootBlock;
class ServerBlock;

class GenericHTTPRequest :
		public HTTPRequest
{
	private:
		const HTTPMethod &m_method;
		URL m_url;
		HTTPVersion m_version;
		HTTPHeaderFields m_headerFields;
		const std::string m_body;
		const Configuration &m_configuration;
		const ServerBlock &m_serverBlock;
		const Optional<LocationBlock const*> &m_locationBlock;

	private:
		GenericHTTPRequest(void);
		GenericHTTPRequest(const GenericHTTPRequest &other);

		GenericHTTPRequest&
		operator =(const GenericHTTPRequest &other);

	public:
		GenericHTTPRequest(const HTTPMethod &method, const URL &url, const HTTPVersion &version, const HTTPHeaderFields &headerFields, const std::string &body, const Configuration &configuration, const ServerBlock &serverBlock, const Optional<LocationBlock const*> &locationBlock);

		virtual
		~GenericHTTPRequest(void);

		const HTTPMethod&
		method() const;

		const URL&
		url() const;

		const std::string&
		resource() const;

		const HTTPVersion&
		version() const;

		const HTTPHeaderFields&
		headers() const;

		const std::string&
		body() const;

		const Configuration&
		configuration() const;

		const ServerBlock&
		server() const;

		const Optional<LocationBlock const*>&
		location() const;
};

#endif /* GENERICHTTPREQUEST_HPP_ */
