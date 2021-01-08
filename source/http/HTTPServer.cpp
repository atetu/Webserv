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

#include <http/HTTPServer.hpp>
#include <io/Socket.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <net/address/InetSocketAddress.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <util/System.hpp>
#include <iostream>

Logger &HTTPServer::LOG = LoggerFactory::get("HTTP Server");

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

	NIOSelector::instance().add(m_socket, *this, NIOSelector::ACCEPT);
}

void
HTTPServer::close(void)
{
	m_socket.close();
}

void
HTTPServer::watchForTimeouts()
{
	typedef std::list<HTTPClient*> lst;

	if (m_clients.empty())
		return;

	unsigned long now = System::currentTimeSeconds();
	for (lst::iterator it = m_clients.begin(); it != m_clients.end();)
	{
		HTTPClient &client = *(*it);
		it++;

		if (client.lastAction() + 30 /* TODO */< now)
		{
			if (LOG.isTraceEnabled())
				LOG.trace() << "Timeout-ed: " << client.socketAddress().hostAddress() << " (fd=" << client.socket().raw() << ")" << std::endl;

			delete &client;
		}
	}
}

Socket&
HTTPServer::socket(void)
{
	return (m_socket);
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

bool
HTTPServer::readable(FileDescriptor &fd)
{
	Socket &serverSocket = static_cast<Socket&>(fd);

	InetSocketAddress socketAddress;
	Socket *socket = serverSocket.accept(&socketAddress);

	HTTPClient &httpClient = *(new HTTPClient(*socket, socketAddress, *this));

//	if (clientFds.size() >= (unsigned long)Configuration::instance().rootBlock().maxActiveClient().orElse(RootBlock::DEFAULT_MAX_ACTIVE_CLIENT))
//	{
//		httpClient.response().status(*HTTPStatus::SERVICE_UNAVAILABLE);
//		httpClient.response().headers().retryAfter(10);
//	}
//
//	LOG.info() << "accepted " << socketAddress.hostAddress() << std::endl;

	m_clients.push_back(&httpClient);

	NIOSelector::instance().add(httpClient.socket(), httpClient, NIOSelector::READ);

	return (false);
}

void
HTTPServer::untrack(HTTPClient &client)
{
	m_clients.remove(&client);
}

HTTPServer::client_list::size_type
HTTPServer::tracked()
{
	return (m_clients.size());
}
