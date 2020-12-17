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
#include <iterator>

BaseBuffer::BaseBuffer() :
		m_storage()
{
}

BaseBuffer::~BaseBuffer()
{
}

void
BaseBuffer::store(char c)
{
	m_storage += c;
}

void
BaseBuffer::store(const std::string &str)
{
	m_storage += str;
}

void
BaseBuffer::store(const void *buffer, size_t len)
{
	m_storage += std::string(static_cast<const char*>(buffer), len);
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
BaseBuffer::peek(char &c) const
{
	if (m_storage.empty())
		return (false);

	c = m_storage[0];

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
