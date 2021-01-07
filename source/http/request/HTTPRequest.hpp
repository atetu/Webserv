/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:51:12 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 18:51:12 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP_
# define HTTPREQUEST_HPP_

#include <config/block/AuthBlock.hpp>
#include <config/block/LocationBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <util/Optional.hpp>
#include <util/URL.hpp>
#include <list>
#include <string>

class Configuration;
class File;

class HTTPRequest
{
	private:
		const Configuration *m_configuration;
		URL m_url;
		const HTTPMethod *m_method;
		const ServerBlock *m_serverBlock;
		const LocationBlock *m_locationBlock;
		HTTPHeaderFields m_headers;
		std::string m_resource;

	public:
		HTTPRequest();
		HTTPRequest(const Configuration &configuration, URL url);
		HTTPRequest(const HTTPRequest &other);

		virtual
		~HTTPRequest();

		HTTPRequest&
		operator=(const HTTPRequest &other);

		const URL&
		url(void) const;

		void
		method(const HTTPMethod &method);

		Optional<const HTTPMethod*>
		method(void) const;

		void
		serverBlock(const ServerBlock &serverBlock);

		Optional<const ServerBlock*>
		serverBlock(void) const;

		void
		locationBlock(const LocationBlock &locationBlock);

		const Optional<const LocationBlock*>
		locationBlock(void) const;

		HTTPHeaderFields&
		headers();

		const std::string&
		resource() const;

		void
		resource(const std::string &resource);

		std::list<const HTTPMethod*>
		allowedMethods();

		Optional<const AuthBlock*>
		authBlock() const;

		std::string
		root(void) const;

		File
		targetFile();

		const Configuration&
		configuration();

		bool
		listing() const;
};

#endif /* HTTPREQUEST_HPP_ */
