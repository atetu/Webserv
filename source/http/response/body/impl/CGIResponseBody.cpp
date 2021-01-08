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
#include <encoding/default/chunk/ChunkEncoder.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/response/body/impl/CGIResponseBody.hpp>
#include <http/response/HTTPResponse.hpp>
#include <io/Socket.hpp>
#include <stddef.h>
#include <util/Number.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <string>

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
	m_cgi.exit();

	NIOSelector::instance().remove(m_cgi.in());
	NIOSelector::instance().remove(m_cgi.out());

	delete &m_bufferedIn;
	delete &m_bufferedOut;
	delete &m_cgi;
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
		return (false);

	buffer.store(m_bufferedOut, true, ChunkEncoder::staticEncode);

	if (isDone())
	{
		buffer.store(ChunkEncoder::ZERO);
		return (true);
	}

	return (false);
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

	m_bufferedOut.read(); // TODO Handle -1

	if (m_headerFieldsParser.state() != HTTPHeaderFieldsParser::S_END)
	{
		char c;

		bool parsed = false;
		while (m_bufferedOut.peek(c))
		{
			m_headerFieldsParser.consume(c);

			if (m_headerFieldsParser.state() == HTTPHeaderFieldsParser::S_END)
			{
				parsed = true;
				break;
			}

			m_bufferedOut.next(c);
		}

		if (parsed)
		{
			m_client.response().headers().merge(m_headerFieldsParser.headerFields(), true);

			Optional<std::string> statusOpt = m_headerFieldsParser.headerFields().get(HTTPHeaderFields::STATUS);
			if (statusOpt.present())
			{
				std::string codePart = statusOpt.get().substr(0, statusOpt.get().find(' '));

				int code = Number::parse<int>(codePart);
				const HTTPStatus *newStatus = HTTPStatus::find(code);

				if (newStatus)
					m_client.response().status(*newStatus);
			}

			m_client.response().headers().chunkedTransferEncoding();
			m_client.filterChain().next();

			NIOSelector::instance().update(m_client.socket(), NIOSelector::WRITE);
		}

		return (false);
	}

	if (m_bufferedOut.hasReadEverything())
		NIOSelector::instance().remove(m_cgi.out());

	return (m_bufferedOut.hasReadEverything());
}

bool
CGIResponseBody::isDone()
{
	return (m_bufferedOut.hasReadEverything() && m_bufferedOut.empty());
}
