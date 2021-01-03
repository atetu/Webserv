/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SubHTTPRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 20:52:18 by ecaceres          #+#    #+#             */
/*   Updated: 2020/01/03 20:52:18 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <http/request/impl/GenericHTTPRequest.hpp>
#include <http/request/impl/SubHTTPRequest.hpp>
#include <util/Optional.hpp>

SubHTTPRequest::SubHTTPRequest(const GenericHTTPRequest &request, const HTTPMethod &method, const std::string &resource) :
		m_request(request),
		m_method(method),
		m_resource(resource)
{
}

SubHTTPRequest::~SubHTTPRequest()
{
	delete &m_request;
}

const HTTPMethod&
SubHTTPRequest::method() const
{
	return (m_request.method());
}

const URL&
SubHTTPRequest::url() const
{
	return (m_request.url());
}

const std::string&
SubHTTPRequest::resource() const
{
	return (m_resource);
}

const HTTPVersion&
SubHTTPRequest::version() const
{
	return (m_request.version());
}

const HTTPHeaderFields&
SubHTTPRequest::headers() const
{
	return (m_request.headers());
}

const std::string&
SubHTTPRequest::body() const
{
	return (m_request.body());
}

const Configuration&
SubHTTPRequest::configuration() const
{
	return (m_request.configuration());
}

const ServerBlock&
SubHTTPRequest::server() const
{
	return (m_request.server());
}

const Optional<const LocationBlock*>&
SubHTTPRequest::location() const
{
	return (m_request.location());
}
