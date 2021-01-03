/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:33:42 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/09 14:58:48 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <exception/Exception.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/enums/HTTPVersion.hpp>
#include <http/handler/HTTPMethodHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <http/response/impl/cgi/CGIHTTPResponse.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <http/route/HTTPFindLocation.hpp>
#include <io/Socket.hpp>
#include <buffer/impl/BaseBuffer.hpp>
#include <buffer/impl/SocketBuffer.hpp>
#include <util/Enum.hpp>
#include <util/Environment.hpp>
#include <util/Optional.hpp>
#include <util/URL.hpp>
#include <algorithm>
#include <map>

HTTPServer::HTTPServer(const std::string &host, short port, const std::list<ServerBlock const*> &serverBlocks) :
		m_host(host),
		m_port(port),
		m_serverBlocks(serverBlocks),
		m_socket(*Socket::create())
{
}

HTTPServer::~HTTPServer(void)
{
	delete &m_socket;
}

void
HTTPServer::start(void)
{
	m_socket.bind(m_host, m_port);
	m_socket.reusable();
	m_socket.listen();
}

void
HTTPServer::terminate(void)
{
	m_socket.close();
}

const Socket&
HTTPServer::socket(void) const
{
	return (m_socket);
}

const std::string&
HTTPServer::host(void) const
{
	return (m_host);
}

short
HTTPServer::port(void) const
{
	return (m_port);
}

const std::list<const ServerBlock*>&
HTTPServer::serverBlocks(void) const
{
	return (m_serverBlocks);
}

const ServerBlock*
HTTPServer::defaultServerBlock(void) const
{
	for (std::list<const ServerBlock*>::const_iterator it = m_serverBlocks.begin(); it != m_serverBlocks.end(); it++)
	{
		const ServerBlock &serverBlock = *(*it);

		if (serverBlock.isDefault().orElse(false))
			return (&serverBlock);
	}

	return (NULL);
}
