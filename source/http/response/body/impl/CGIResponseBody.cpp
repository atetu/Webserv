/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseBody.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 17:24:51 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 17:24:51 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/HTTPClient.hpp>
#include <http/response/body/impl/CGIResponseBody.hpp>
#include <io/Socket.hpp>
#include <util/Singleton.hpp>

CGIResponseBody::CGIResponseBody(HTTPClient &client, CommonGatewayInterface &cgi) :
		m_client(client),
		m_cgi(cgi),
		m_bufferedIn(*FileDescriptorBuffer::from(m_cgi.in(), FileDescriptorBuffer::NOTHING)),
		m_bufferedOut(*FileDescriptorBuffer::from(m_cgi.out(), FileDescriptorBuffer::NOTHING))
{
	NIOSelector::instance().add(m_cgi.in(), *this, NIOSelector::WRITE);
	NIOSelector::instance().add(m_cgi.out(), *this, NIOSelector::READ);
}

CGIResponseBody::~CGIResponseBody()
{
	NIOSelector::instance().remove(m_cgi.in());
	NIOSelector::instance().remove(m_cgi.out());
}

bool
CGIResponseBody::isSelfManaged() const
{
	return (true);
}

bool
CGIResponseBody::store(BaseBuffer &buffer)
{
	if (m_headerFieldsParser.state() != HTTPHeaderFieldsParser::S_END)
		return (true);

	buffer.store(m_bufferedOut);

	return (isDone());
}

bool
CGIResponseBody::writable(FileDescriptor &fd)
{
	(void)fd;

	m_bufferedIn.write();

	if (m_bufferedIn.empty())
	{
		NIOSelector::instance().remove(m_cgi.in());
		return (true);
	}

	return (false);
}

bool
CGIResponseBody::readable(FileDescriptor &fd)
{
	(void)fd;

	m_bufferedOut.read();

	if (m_headerFieldsParser.state() != HTTPHeaderFieldsParser::S_END)
	{
		char c;

		bool parsed = false;
		while (m_bufferedOut.peek(c))
		{
			std::cout << c << std::flush;
			m_headerFieldsParser.consume(c);

			if (m_headerFieldsParser.state() == HTTPHeaderFieldsParser::S_END)
			{
				std::cout << "PARSED" << std::endl;
				parsed = true;
				break;
			}

			m_bufferedOut.next(c);
		}

		if (parsed)
		{
			NIOSelector::instance().update(m_client.socket(), NIOSelector::WRITE);
			m_client.filterChain().next();
		}

		return (false);
	}

	return (isDone());
}

bool
CGIResponseBody::isDone()
{
	if (m_cgi.running())
		return (!m_bufferedIn.empty() && m_bufferedOut.empty());

	return (m_bufferedOut.empty());
}
