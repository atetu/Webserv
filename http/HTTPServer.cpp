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

HTTPServer::HTTPServer(const std::string &host, short port, const std::list<ServerBlock const*> &serverBlocks) :
		m_host(host),
		m_port(port),
		m_serverBlocks(serverBlocks),
		m_socket(*Socket::create())
{
}

HTTPServer::~HTTPServer()
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
HTTPServer::host() const
{
	return m_host;
}

short
HTTPServer::port() const
{
	return m_port;
}

const std::list<const ServerBlock*>&
HTTPServer::serverBlocks() const
{
	return m_serverBlocks;
}
