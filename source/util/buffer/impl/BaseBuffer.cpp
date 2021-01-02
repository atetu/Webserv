/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseBuffer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 20:16:07 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 20:16:07 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/buffer/impl/BaseBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <typeinfo>

BaseBuffer::BaseBuffer() :
		m_storage(),
		m_maxSize(std::string::npos)
{
}

BaseBuffer::BaseBuffer(size_type maxSize) :
		m_storage(),
		m_maxSize(maxSize)
{
}

BaseBuffer::BaseBuffer(const std::string &storage, size_type maxSize) :
		m_storage(storage),
		m_maxSize(maxSize)
{
}

BaseBuffer::~BaseBuffer()
{
}

BaseBuffer&
BaseBuffer::store(char c)
{
	m_storage += c;

	return (*this);
}

BaseBuffer&
BaseBuffer::store(const std::string &str)
{
	m_storage += str;

	return (*this);
}

BaseBuffer&
BaseBuffer::store(BaseBuffer &buffer, bool andErase)
{
	size_type capacity = std::min(this->capacity(), buffer.capacity());

	if (capacity)
	{

		m_storage += buffer.storage().substr(0, capacity);

		if (andErase)
			buffer.storage().erase(0, capacity);
	}

	return (*this);
}

BaseBuffer&
BaseBuffer::store(const void *buffer, size_t len)
{
	m_storage += std::string(static_cast<const char*>(buffer), len);

	return (*this);
}

bool
BaseBuffer::next(char &c)
{
	if (m_storage.empty())
		return (false);

	c = m_storage[0];

	m_storage.erase(0, 1);

	return (true);
}

bool
BaseBuffer::peek(char &c, size_t index) const
{
	if (m_storage.empty() || index >= m_storage.size())
		return (false);

	c = m_storage[index];

	return (true);
}

bool
BaseBuffer::next(std::string &str, bool crlf)
{
	if (m_storage.empty())
		return (false);

	for (std::string::iterator it = m_storage.begin(); it != m_storage.end(); it++)
	{
		if (*it == '\n')
		{
			str = std::string(m_storage.begin(), it);

			m_storage.erase(m_storage.begin(), it);

			return (true);
		}
	}

	return (false);
}

size_t
BaseBuffer::size() const
{
	return (m_storage.size());
}

bool
BaseBuffer::empty() const
{
	return (m_storage.empty());
}

void
BaseBuffer::clear()
{
	m_storage.clear();
}

size_t
BaseBuffer::capacity() const
{
	if (m_storage.size() >= m_maxSize)
		return (0);

	return (m_maxSize - m_storage.size());
}
