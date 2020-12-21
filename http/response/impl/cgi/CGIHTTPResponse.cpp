/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHTTPResponse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 21:09:07 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/20 21:09:07 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTP.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <http/response/impl/cgi/CGIHTTPResponse.hpp>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <vector>

CGIHTTPResponse::CGIHTTPResponse(const HTTPStatusLine &statusLine, CommonGatewayInterface &cgi) :
		m_statusLine(statusLine),
		m_cgi(cgi),
		m_inBuffer(FileDescriptorBuffer::from(cgi.in(), FileDescriptorBuffer::NOTHING, 4096)),
		m_outBuffer(FileDescriptorBuffer::from(cgi.out(), FileDescriptorBuffer::NOTHING, 4096)),
		m_state(NONE)
{
}

CGIHTTPResponse::~CGIHTTPResponse()
{
	m_cgi.exit();

	DeleteHelper::pointer<FileDescriptorBuffer>(m_inBuffer);
	DeleteHelper::pointer<FileDescriptorBuffer>(m_outBuffer);

	delete &m_cgi;
}

bool
CGIHTTPResponse::write(SocketBuffer &socketBuffer)
{
	if (m_state == NONE)
		m_state = STATUS_LINE;

	static int running = -1;
	if (m_cgi.running() != running)
	{
		running = m_cgi.running();
		std::cout << "running: " << m_cgi.running() << ", " << m_outBuffer->storage() << std::endl;
	}

	switch (m_state)
	{
		case STATUS_LINE:
			socketBuffer.store(m_statusLine.format());
			socketBuffer.store(HTTP::CRLF);

			m_state = BODY;

			break;

		case BODY:
			socketBuffer.store(*m_outBuffer);
			socketBuffer.send();

			if (!m_cgi.running())
				m_state = FLUSHING;

			break;

		case FLUSHING:
			if (socketBuffer.send() < 0 || socketBuffer.size() == 0)
				m_state = FINISHED;

			break;

		default:
			break;
	}

	return (m_state == FINISHED);
}

void
CGIHTTPResponse::readFileDescriptors(fdb_vector &out)
{
	out.push_back(m_outBuffer);
}

void
CGIHTTPResponse::writeFileDescriptors(fdb_vector &out)
{
	out.push_back(m_inBuffer);
}

HTTPStatusLine
CGIHTTPResponse::statusLine() const
{
	return (m_statusLine);
}

CGIHTTPResponse::State
CGIHTTPResponse::state() const
{
	return (m_state);
}
