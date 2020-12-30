/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:24:32 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/20 17:24:32 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HPP_
# define FT_HPP_

#include <stddef.h>

namespace ft
{
	void*
	memcpy(void *dest, const void *src, size_t len);

	void*
	memset(void *dest, char val, size_t len);

	void*
	bzero(void *dest, size_t len);

	size_t
	strlen(const char *str);

	char*
	strdup(const char *str);

	char*
	strchr(char *str, char c);
}

#endif /* FT_HPP_ */
