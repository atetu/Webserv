/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileBuffer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 18:24:26 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 18:24:26 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/buffer/impl/FileBuffer.hpp>
#include <algorithm>

FileBuffer::FileBuffer(FileDescriptor &fileDescriptor, int actionOnDestroy, size_type maxSize) :
		BaseBuffer(),
		m_fd(fileDescriptor),
		m_actionOnDestroy(actionOnDestroy),
		m_maxSize(maxSize),
		m_readEverything(false)
{
}

FileBuffer::~FileBuffer()
{
	if (m_actionOnDestroy & CLOSE)
		close();

	if (m_actionOnDestroy & DELETE)
		delete &m_fd;
}

ssize_t
FileBuffer::read(size_t len)
{
	size_t capacity;

	if (m_maxSize == std::string::npos)
		capacity = len;
	else
		capacity = this->capacity();

	if (capacity)
	{
		char rbuffer[capacity];
		ssize_t r = m_fd.read(rbuffer, std::min(capacity, len));

		if (r >= 0)
			BaseBuffer::store(rbuffer, r);

		m_readEverything = r == 0;

		return (r);
	}

	return (0);
}

ssize_t
FileBuffer::write(size_t len)
{
	ssize_t r = m_fd.write(m_storage.data(), std::min(m_storage.length(), len));

	if (r > 0)
		m_storage.erase(0, r);

	return (r);
}

void
FileBuffer::storeFrom(FileBuffer &buffer, bool andClear)
{
	size_type capacity = std::min(this->capacity(), buffer.capacity());

	if (capacity)
	{
		m_storage += buffer.storage().substr(0, capacity);

		buffer.storage().erase(0, capacity);
	}

	if (andClear)
		buffer.clear();
}

size_t
FileBuffer::capacity() const
{
	if (m_storage.size() >= m_maxSize)
		return (0);

	return (m_maxSize - m_storage.size());
}

void
FileBuffer::close(void)
{
	m_fd.close();
}

FileBuffer*
FileBuffer::from(FileDescriptor &fileDescriptor, int actionOnDestroy, size_type maxSize)
{
	return (new FileBuffer(fileDescriptor, actionOnDestroy, maxSize));
}
