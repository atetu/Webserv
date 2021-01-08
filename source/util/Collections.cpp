/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Collections.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 22:04:14 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 22:04:14 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/Collections.hpp>

Collections::Collections(void)
{
}

Collections::Collections(const Collections &other)
{
	(void)other;
}

Collections::~Collections(void)
{
}

Collections
Collections::operator =(const Collections &other)
{
	(void)other;

	return (*this);
}
