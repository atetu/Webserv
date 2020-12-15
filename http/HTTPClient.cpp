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
#include <stddef.h>
#include <sys/unistd.h>
#include <util/System.hpp>

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

HTTPClient::~HTTPClient(void)
{
	std::cout << "~HTTPClient(): m_request=" << (void*)m_request << ", m_response=" << (void*)m_response << std::endl;

	if (m_request)
		delete m_request;

	if (m_response)
		delete m_response;

	::close(m_fd);
}

void
HTTPClient::updateLastAction()
{
	long time = System::currentTimeSeconds();

	if (time)
		m_lastAction = time;
}
