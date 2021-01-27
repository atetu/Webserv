/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileResponseBody.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 17:14:47 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/27 10:55:52 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <http/response/body/impl/FileResponseBody.hpp>
#include <util/Singleton.hpp>
#include <iostream>

FileResponseBody::FileResponseBody(FileDescriptorBuffer &fdBuffer) :
		m_fdBuffer(fdBuffer),
		m_error(false)
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
	
	if (r == -1)
	{	
		m_error = true;
		return (true);
	}	
	return (isDone());
}

bool
FileResponseBody::isDone()
{
	return (m_fdBuffer.hasReadEverything() && m_fdBuffer.empty());
}

bool
FileResponseBody::error()
{
	return (m_error);
}