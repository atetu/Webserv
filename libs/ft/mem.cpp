/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:24:54 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/20 17:24:54 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

namespace ft
{
	void*
	memcpy(void *dest, const void *src, size_t len)
	{
		void *start = dest;

		if (src && dest)
		{
			while (len >= 8)
			{
				*((unsigned long*&)dest)++ = *((const unsigned long*&)src)++;
				len -= 8;
			}

			while (len--)
				*((unsigned char*&)dest)++ = *((const unsigned char*&)src)++;
		}

		return (start);
	}

	void*
	memset(void *dest, char val, size_t len)
	{
		void *start = dest;

		if (dest)
		{
			if (len >= 8)
			{
				unsigned long xval = val << 56 + val << 48 + val << 40 + val << 32 + val << 24 + val << 16 + val << 8 + val;

				while (len >= 8)
				{
					*(((unsigned long*&)dest)++) = xval;
					len -= 8;
				}
			}

			while (len--)
				*(((unsigned char*&)dest)++) = val;
		}

		return (start);
	}

	void*
	bzero(void *dest, size_t len)
	{
		void *start = dest;

		if (dest)
		{
			while (len >= 8)
			{
				*(((unsigned long*&)dest)++) = 0;
				len -= 8;
			}

			while (len--)
				*(((unsigned char*&)dest)++) = 0;
		}

		return (start);
	}
}
