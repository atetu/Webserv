/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GenericHTTPRequest.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 13:18:11 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 17:03:13 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/request/impl/GenericHTTPRequest.hpp>

class HTTPVersion;

GenericHTTPRequest::GenericHTTPRequest(const HTTPMethod &method, const URL &url, const HTTPVersion &version, const HTTPHeaderFields &headerFields, const std::string &body, const Configuration &configuration, const ServerBlock &serverBlock, const Optional<LocationBlock const*> &locationBlock) :
		m_method(method),
		m_url(url),
		m_version(version),
		m_headerFields(headerFields),
		m_body(body),
		m_configuration(configuration),
		m_serverBlock(serverBlock),
		m_locationBlock(locationBlock)
{
}

GenericHTTPRequest::~GenericHTTPRequest(void)
{
}

const HTTPMethod&
GenericHTTPRequest::method() const
{
	return (m_method);
}

const URL&
GenericHTTPRequest::url() const
{
	return (m_url);
}

const std::string&
GenericHTTPRequest::resource() const
{
	return (m_url.path());
}

const HTTPVersion&
GenericHTTPRequest::version() const
{
	return (m_version);
}

const HTTPHeaderFields&
GenericHTTPRequest::headers() const
{
	return (m_headerFields);
}

const std::string&
GenericHTTPRequest::body() const
{
	return (m_body);
}

const Configuration&
GenericHTTPRequest::configuration() const
{
	return (m_configuration);
}

const ServerBlock&
GenericHTTPRequest::server() const
{
	return (m_serverBlock);
}

const Optional<const LocationBlock*>&
GenericHTTPRequest::location() const
{
	return (m_locationBlock);
}
