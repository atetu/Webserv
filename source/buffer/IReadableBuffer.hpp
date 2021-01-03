/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IReadableBuffer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 18:19:03 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/29 18:19:03 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREADABLEBUFFER_HPP_
# define IREADABLEBUFFER_HPP_

#include <stddef.h>
#include <buffer/IBuffer.hpp>

class IReadableBuffer :
		public IBuffer
{
	public:
		virtual
		~IReadableBuffer(void)
		{
		}

		virtual bool
		next(char &c) = 0;

		virtual bool
		peek(char &c, size_t index = 0) const = 0;

		virtual bool
		next(std::string &str, bool crlf = false) = 0;
};

#endif /* IREADABLEBUFFER_HPP_ */
