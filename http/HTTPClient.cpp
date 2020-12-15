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

#include <exception/Exception.hpp>
#include <http/HTTPClient.hpp>
#include <stddef.h>
#include <util/System.hpp>

HTTPClient::~HTTPClient(void)
{
	if (m_request)
		delete m_request;

	if (m_response)
		delete m_response;
}

HTTPClient::HTTPClient(int fd) :
		m_fd(fd),
		m_in(fd, false),
		m_out(fd, false),
		m_parser(),
		m_lastAction(),
		m_request(NULL),
		m_response(NULL)
{
	updateLastAction();
}

void
HTTPClient::updateLastAction()
{
	long time = System::currentTimeSeconds();

	if (time)
		m_lastAction = time;
}

void
HTTPClient::header(HTTPHeaderParser headerParser)
{
	m_headerParser.push_back(headerParser);
}

std::vector<HTTPHeaderParser> 
HTTPClient::getHeader()
{
	return (m_headerParser);
}
