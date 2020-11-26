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

#include <string>

class BaseBuffer
{
	protected:
		std::string m_buffer;

	public:
		BaseBuffer() :
				m_buffer()
		{
		}

		BaseBuffer(const std::string &buffer) :
				m_buffer(buffer)
		{
		}

		BaseBuffer(const BaseBuffer &other) :
				m_buffer(other.m_buffer)
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
				m_buffer = other.m_buffer;

			return (*this);
		}

		void
		store(char c)
		{
			m_buffer += c;
		}

		void
		store(const std::string &str)
		{
			m_buffer += str;
		}

		void
		store(const void *buffer, size_t len)
		{
			m_buffer += std::string(static_cast<const char*>(buffer), len);
		}

		bool
		getChar(char &c)
		{
			if (m_buffer.empty())
				return (false);

			c = m_buffer[0];

			m_buffer.erase(0, 1);

			return (true);
		}

		bool
		getLine(std::string &str, bool crlf = false)
		{
			if (m_buffer.empty())
				return (false);

			for (std::string::iterator it = m_buffer.begin(); it != m_buffer.end(); it++)
			{
				if (*it == '\n')
				{
					str = std::string(m_buffer.begin(), it);

					m_buffer.erase(m_buffer.begin(), it);

					return (true);
				}
			}

			return (false);
		}

		size_t
		size()
		{
			return (m_buffer.size());
		}
};

#endif /* BASEBUFFER_HPP_ */
