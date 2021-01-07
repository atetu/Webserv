/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileResponseBody.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 17:14:47 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 17:14:47 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <http/response/body/impl/FileResponseBody.hpp>
#include <io/FileDescriptor.hpp>

FileResponseBody::FileResponseBody(FileDescriptorBuffer &fdBuffer) :
		m_fdBuffer(fdBuffer)
{
}

FileResponseBody::~FileResponseBody()
{
	delete &m_fdBuffer;
}

void
FileResponseBody::io(FileDescriptorBuffer **in, FileDescriptorBuffer **out)
{
	(void)out;

	*in = &m_fdBuffer;
}

bool
FileResponseBody::store(BaseBuffer &buffer)
{
	buffer.store(m_fdBuffer);

	return (isDone());
}

bool
FileResponseBody::isDone()
{
	return (m_fdBuffer.hasReadEverything() && m_fdBuffer.empty());
}
