/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPClient.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 22:15:19 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 22:15:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCLIENT_HPP_
# define HTTPCLIENT_HPP_

#include <http/HTTPRequestParser.hpp>
#include <io/Socket.hpp>

class HTTPServer;
class SocketBuffer;
class Socket;
class HTTPRequest;
class HTTPResponse;

class HTTPClient
{
	private:
		Socket &m_socket;
		SocketBuffer &m_in;
		SocketBuffer &m_out;
		const HTTPServer &m_server;
		HTTPRequestParser m_parser;
		unsigned long m_lastAction;
		HTTPRequest *m_request;
		HTTPResponse *m_response;

		HTTPClient(void);
		HTTPClient(const HTTPClient &other);

		HTTPClient&
		operator =(const HTTPClient &other);

	public:
		HTTPClient(Socket &socket, const HTTPServer &server);

		virtual
		~HTTPClient();

		void
		updateLastAction();

		void
		header(HTTPHeaderParser headerParser);

		// std::vector<HTTPHeaderParser> 
		// getHeader();

		inline SocketBuffer&
		in()
		{
			return (m_in);
		}

		inline SocketBuffer&
		out()
		{
			return (m_out);
		}

		inline HTTPRequestParser&
		parser()
		{
			return (m_parser);
		}

		inline unsigned long&
		lastAction()
		{
			return (m_lastAction);
		}

		inline HTTPRequest*&
		request(void)
		{
			return (m_request);
		}

		inline HTTPResponse*&
		response(void)
		{
			return (m_response);
		}

		inline Socket&
		socket(void)
		{
			return (m_socket);
		}
};

#endif /* HTTPCLIENT_HPP_ */
