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
#include <util/buffer/IOBuffer.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPHeaderParser.hpp>

class HTTPClient
{
	private:
		int m_fd;
		IOBuffer m_in;
		IOBuffer m_out;
		HTTPRequestParser m_parser;
		std::vector<HTTPHeaderParser> m_headerParser;
		unsigned long m_lastAction;
		HTTPRequest *m_request;
		HTTPResponse *m_response;

		HTTPClient(void);
		HTTPClient(const HTTPClient &other);

		HTTPClient&
		operator =(const HTTPClient &other);

	public:
		HTTPClient(int fd);

		virtual
		~HTTPClient();

		void
		updateLastAction();

		void
		header(HTTPHeaderParser headerParser);

		std::vector<HTTPHeaderParser> 
		getHeader();

		inline IOBuffer&
		in()
		{
			return (m_in);
		}

		inline IOBuffer&
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
};

#endif /* HTTPCLIENT_HPP_ */
