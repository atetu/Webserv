/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 13:18:11 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/20 13:18:11 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTPRequest.hpp>
#include <sstream>

HTTPRequest::HTTPRequest(const HTTPMethod &method, const URL &url, const HTTPVersion &version, const HTTPHeaderFields &headerFields, const Configuration &configuration, const RootBlock &rootBlock, const ServerBlock &serverBlock, const LocationBlock &locationBlock) :
		m_method(method),
		m_url(url),
		m_version(version),
		m_headerFields(headerFields),
		m_configuration(configuration),
		m_rootBlock(rootBlock),
		m_serverBlock(serverBlock),
		m_locationBlock(locationBlock)
{
}

HTTPRequest::~HTTPRequest(void)
{
}
