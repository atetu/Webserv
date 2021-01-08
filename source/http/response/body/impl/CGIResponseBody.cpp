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
#include <http/response/body/impl/CGIResponseBody.hpp>

CGIResponseBody::CGIResponseBody(CommonGatewayInterface &cgi) :
		m_cgi(cgi),
		m_bufferedIn(*FileDescriptorBuffer::from(m_cgi.in(), FileDescriptorBuffer::NOTHING)),
		m_bufferedOut(*FileDescriptorBuffer::from(m_cgi.out(), FileDescriptorBuffer::NOTHING))
{
}

CGIResponseBody::~CGIResponseBody()
{
}

bool
CGIResponseBody::isSelfManaged() const
{
	return (true);
}

bool
CGIResponseBody::store(BaseBuffer &buffer)
{
	buffer.store(m_bufferedOut);

	return (isDone());
}

bool
CGIResponseBody::writable(FileDescriptor &fd)
{
	(void)fd;

	m_bufferedIn.write();

	return (m_bufferedIn.empty());
}

bool
CGIResponseBody::readable(FileDescriptor &fd)
{
	(void)fd;

	m_bufferedOut.read();

	return (isDone());
}

bool
CGIResponseBody::isDone()
{
	if (m_cgi.running())
		return (!m_bufferedIn.empty() && m_bufferedOut.empty());

	return (m_bufferedOut.empty());
}