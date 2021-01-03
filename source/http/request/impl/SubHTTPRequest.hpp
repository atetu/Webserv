/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SubHTTPRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 20:52:18 by ecaceres          #+#    #+#             */
/*   Updated: 2020/01/03 20:52:18 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SUBHTTPREQUEST_HPP_
# define SUBHTTPREQUEST_HPP_

#include <config/block/LocationBlock.hpp>
#include <http/request/HTTPRequest.hpp>
#include <util/Optional.hpp>
#include <string>

class GenericHTTPRequest;
class HTTPMethod;

class SubHTTPRequest :
		public HTTPRequest
{
	private:
		const GenericHTTPRequest &m_request;
		const HTTPMethod &m_method;
		std::string m_resource;

	private:
		SubHTTPRequest();
		SubHTTPRequest(const SubHTTPRequest &other);

		SubHTTPRequest&
		operator=(const SubHTTPRequest &other);

	public:
		SubHTTPRequest(const GenericHTTPRequest &request, const HTTPMethod &method, const std::string &resource);

		virtual
		~SubHTTPRequest();

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

#endif /* SUBHTTPREQUEST_HPP_ */
