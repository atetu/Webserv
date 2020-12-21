/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPClient.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 22:15:19 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 22:15:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTPClient.hpp>
#include <http/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <stddef.h>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <util/System.hpp>

HTTPClient::HTTPClient(Socket &socket, InetSocketAddress socketAddress, const HTTPServer &server) :
		m_socket(socket),
		m_socketAddress(socketAddress),
		m_in(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
		m_out(*SocketBuffer::from(socket, FileDescriptorBuffer::NOTHING)),
		m_server(server),
		m_parser(),
		m_lastAction(),
		m_request(NULL),
		m_response(NULL)
{
	updateLastAction();
}

HTTPClient::~HTTPClient(void)
{
	delete &m_in;
	delete &m_out;
	delete &m_socket;

	DeleteHelper::pointer<HTTPRequest>(m_request);
	DeleteHelper::pointer<HTTPResponse>(m_response);
}

void
HTTPClient::updateLastAction()
{
	long time = System::currentTimeSeconds();

	if (time)
		m_lastAction = time;
}
