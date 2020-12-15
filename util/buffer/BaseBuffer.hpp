/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseBuffer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 22:24:27 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 22:24:27 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASEBUFFER_HPP_
# define BASEBUFFER_HPP_

#include <stddef.h>
#include <util/buffer/IReadableBuffer.hpp>
#include <util/buffer/IWritableBuffer.hpp>
#include <iterator>
#include <string>

class BaseBuffer :
		public IWritableBuffer,
		public IReadableBuffer
{
	public:
		typedef std::string::size_type size_type;

	protected:
		std::string m_storage;

	public:
		BaseBuffer() :
				m_storage()
		{
		}

		BaseBuffer(const std::string &buffer) :
				m_storage(buffer)
		{
		}

		BaseBuffer(const BaseBuffer &other) :
				m_storage(other.m_storage)
		{
		}

		virtual
		~BaseBuffer()
		{
		}

		BaseBuffer&
		operator=(const BaseBuffer &other)
		{
			if (this != &other)
				m_storage = other.m_storage;

			return (*this);
		}

		void
		store(char c)
		{
			m_storage += c;
		}

		void
		store(const std::string &str)
		{
			m_storage += str;
		}

		void
		store(const void *buffer, size_t len)
		{
			m_storage += std::string(static_cast<const char*>(buffer), len);
		}

		virtual bool
		next(char &c)
		{
			if (m_storage.empty())
				return (false);

			c = m_storage[0];

			m_storage.erase(0, 1);

			return (true);
		}

		virtual bool
		peek(char &c) const
		{
			if (m_storage.empty())
				return (false);

			c = m_storage[0];

			return (true);
		}

		virtual bool
		next(std::string &str, bool crlf = false)
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
		size() const
		{
			return (m_storage.size());
		}

		bool
		empty() const
		{
			return (m_storage.empty());
		}

		void
		clear()
		{
			m_storage.clear();
		}

		inline std::string&
		storage()
		{
			return (m_storage);
		}

		inline const std::string&
		storage() const
		{
			return (m_storage);
		}
};

#endif /* BASEBUFFER_HPP_ */
