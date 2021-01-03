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
#include <util/Optional.hpp>
#include <string>

class Configuration;
class HTTPHeaderFields;
class HTTPMethod;
class HTTPVersion;
class ServerBlock;
class URL;

class HTTPRequest
{
	public:
		virtual
		~HTTPRequest(void);

		std::string
		root(void) const;

		bool
		needAuth() const;

		const AuthBlock*
		auth() const;

		virtual const HTTPMethod&
		method() const = 0;

		virtual const URL&
		url() const = 0;

		virtual const std::string&
		resource() const = 0;

		virtual const HTTPVersion&
		version() const = 0;

		virtual const HTTPHeaderFields&
		headers() const = 0;

		virtual const std::string&
		body() const = 0;

		virtual const Configuration&
		configuration() const = 0;

		virtual const ServerBlock&
		server() const = 0;

		virtual const Optional<LocationBlock const*>&
		location() const = 0;
};

#endif /* HTTPREQUEST_HPP_ */
