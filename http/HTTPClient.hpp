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

class IOBuffer;

class HTTPRequest;
class HTTPResponse;

class HTTPClient
{
	private:
		IOBuffer *m_in;
		IOBuffer *m_out;
		HTTPRequestParser m_parser;
		unsigned long m_lastAction;
		HTTPRequest *m_request;
		HTTPResponse *m_response;

	public:
		HTTPClient();

		virtual
		~HTTPClient();
};

#endif /* HTTPCLIENT_HPP_ */
