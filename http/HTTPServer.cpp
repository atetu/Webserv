/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:33:42 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/24 15:25:29 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTPServer.hpp>

HTTPServer::HTTPServer(void) :
		m_port(-1),
		m_servers(),
		m_socketServer()
{
}

HTTPServer::HTTPServer(int port, const std::vector<ServerBlock> &servers) : // pourquoi on se retrouve avec un vecteur et pas juste un ServerBlock?
		m_port(port),
		m_servers(servers)
{
}

HTTPServer::HTTPServer(const HTTPServer &other) :
		m_port(other.m_port),
		m_servers(other.m_servers),
		m_socketServer(other.m_socketServer)
{
}

HTTPServer::~HTTPServer()
{
}

HTTPServer&
HTTPServer::operator =(const HTTPServer &other)
{
	if (this != &other)
	{
		m_port = other.m_port;
		m_servers = other.m_servers;
		m_socketServer = other.m_socketServer;
	}

	return (*this);
}

void
HTTPServer::prepare(void)
{
	m_socketServer.bind(m_port);
	m_socketServer.reusable();
	m_socketServer.listen();
}

void
HTTPServer::unprepare(void)
{
	m_socketServer.close();
}

const SocketServer&
HTTPServer::serverSocket(void) const
{
	return (m_socketServer);
}
