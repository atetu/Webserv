/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileResponseBody.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 17:14:47 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/27 12:21:55 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <http/response/body/impl/FileResponseBody.hpp>
#include <util/Singleton.hpp>
#include <iostream>

FileResponseBody::FileResponseBody(FileDescriptorBuffer &fdBuffer, size_t contentLength) :
		m_fdBuffer(fdBuffer),
		m_contentLength(contentLength),
		m_stored(0)
{
	NIOSelector::instance().add(m_fdBuffer.descriptor(), *this, NIOSelector::READ);	
}

FileResponseBody::~FileResponseBody()
{
	NIOSelector::instance().remove(m_fdBuffer.descriptor());

	delete &m_fdBuffer;
}

bool
FileResponseBody::store(BaseBuffer &buffer)
{
	buffer.store(m_fdBuffer);

	return (isDone());
}

bool
FileResponseBody::readable(FileDescriptor &fd)
{
	(void)fd;

	ssize_t r = m_fdBuffer.read(); // TODO Need check
	m_stored += r;
	if (r == -1)
	{	
		size_t diff = m_contentLength - m_stored;
		if (diff)
			m_stored += m_fdBuffer.storeZeros(diff);
	}	
	return (isDone());
}

bool
FileResponseBody::isDone()
{
	return (m_fdBuffer.hasReadEverything() && m_fdBuffer.empty());
}
