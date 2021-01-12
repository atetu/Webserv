/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGITask.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 16:41:55 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/12 16:41:55 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/cgi/task/CGITask.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/body/impl/CGIResponseBody.hpp>
#include <http/response/HTTPResponse.hpp>
#include <io/Socket.hpp>
#include <sys/types.h>
#include <util/Number.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <string>

CGITask::CGITask(HTTPClient &client, CommonGatewayInterface &cgi) :
		m_client(client),
		m_cgi(cgi),
		m_bufferedOut(*FileDescriptorBuffer::from(m_cgi.out(), FileDescriptorBuffer::NOTHING)),
		wroteBodyUpTo(),
		m_finished(false)
{
	if (m_client.request().method().get()->hasBody())
		NIOSelector::instance().add(m_cgi.in(), *this, NIOSelector::WRITE);
	else
		m_cgi.in().close();

	NIOSelector::instance().add(m_cgi.out(), *this, NIOSelector::READ);
}

CGITask::~CGITask()
{
	m_cgi.exit();

	NIOSelector::instance().remove(m_cgi.in());
	NIOSelector::instance().remove(m_cgi.out());

	delete &m_bufferedOut;
	delete &m_cgi;
}

bool
CGITask::running()
{
	return (!m_finished);
}

bool
CGITask::writable(FileDescriptor &fd)
{
	if (wroteBodyUpTo == m_client.body().length())
		return (true);

	ssize_t wrote = fd.write(m_client.body().c_str() + wroteBodyUpTo, m_client.body().length() - wroteBodyUpTo);

	if (wrote <= 0)
	{
		m_cgi.in().close();
		NIOSelector::instance().remove(m_cgi.in());
		return (true);
	}

	wroteBodyUpTo += wrote;
	if (wroteBodyUpTo == m_client.body().length())
	{
		m_cgi.in().close();
		NIOSelector::instance().remove(m_cgi.in());
		return (true);
	}

	return (false);
}

bool
CGITask::readable(FileDescriptor &fd)
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
			m_client.response().headers().merge(m_headerFieldsParser.headerFields());

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
			m_client.response().body(new CGIResponseBody(m_client, *this));
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
CGITask::isDone()
{
	return (m_bufferedOut.hasReadEverything() && m_bufferedOut.empty());
}

bool
CGITask::hasReadHeaders()
{
	return (m_headerFieldsParser.state() != HTTPHeaderFieldsParser::S_END);
}

FileDescriptorBuffer&
CGITask::out()
{
	return (m_bufferedOut);
}
