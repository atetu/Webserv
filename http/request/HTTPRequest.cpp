/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 13:18:11 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 17:03:13 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <http/request/HTTPRequest.hpp>
#include <map>

class HTTPVersion;

HTTPRequest::HTTPRequest(const HTTPMethod &method, const URL &url, const HTTPVersion &version, const HTTPHeaderFields &headerFields, const std::string body, const Configuration &configuration, const RootBlock &rootBlock, const ServerBlock &serverBlock, const Optional<LocationBlock const*> &locationBlock) :
		m_method(method),
		m_url(url),
		m_version(version),
		m_headerFields(headerFields),
		m_body(body),
		m_configuration(configuration),
		m_rootBlock(rootBlock),
		m_serverBlock(serverBlock),
		m_locationBlock(locationBlock)
{
}

HTTPRequest::~HTTPRequest(void)
{
}

std::string
HTTPRequest::root(void) const
{
	if (m_locationBlock.present())
	{
		const LocationBlock &locationBlock = *m_locationBlock.get();

		if (locationBlock.root().present())
			return (locationBlock.root().get());
	}

	if (m_serverBlock.root().present())
		return (m_serverBlock.root().get());

	if (m_rootBlock.root().present())
		return (m_rootBlock.root().get());

	return ("./");
}

std::string
HTTPRequest::getLocation()
{
	std::map<std::string, std::string>::const_iterator it = m_headerFields.storage().find("Location");
	if (it != m_headerFields.storage().end())
		return (it->second);
	else
		return (this->root());
}
