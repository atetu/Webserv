/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IWritableBuffer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 18:19:03 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/29 18:19:03 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IWRITABLEBUFFER_HPP_
# define IWRITABLEBUFFER_HPP_

#include <stddef.h>
#include <buffer/IBuffer.hpp>

class IWritableBuffer :
		public IBuffer
{
	public:
		virtual
		~IWritableBuffer(void)
		{
		}

		virtual IWritableBuffer&
		store(char c) = 0;

		virtual IWritableBuffer&
		store(const std::string &str) = 0;

		virtual IWritableBuffer&
		store(const void *buffer, size_t len) = 0;

		virtual size_t
		size() const = 0;

		virtual void
		clear() = 0;
};

#endif /* IWRITABLEBUFFER_HPP_ */
