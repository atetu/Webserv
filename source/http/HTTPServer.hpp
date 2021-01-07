/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:33:42 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 14:33:42 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP_
# define HTTPSERVER_HPP_

#include <config/block/ServerBlock.hpp>
#include <list>
#include <string>

class HTTPClient;
class Socket;

class HTTPServer
{
	private:
		std::string m_host;
		short m_port;
		std::list<ServerBlock const*> m_serverBlocks;
		Socket &m_socket;

	private:
		HTTPServer(void);
		HTTPServer(const HTTPServer &other);

		HTTPServer&
		operator=(const HTTPServer &other);

	public:
		HTTPServer(const std::string &host, short port, const std::list<ServerBlock const*> &serverBlocks);

		virtual
		~HTTPServer(void);

		void
		start(void);

		void
		terminate(void);

		const Socket&
		socket(void) const;

		const std::string&
		host(void) const;

		short
		port(void) const;

		const std::list<const ServerBlock*>&
		serverBlocks(void) const;

		ServerBlock const*
		defaultServerBlock(void) const;
};

#endif /* HTTPSERVER_HPP_ */
