/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodHandlingFilter.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 00:28:17 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 00:28:17 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/between/MethodHandlingFilter.hpp>
#include <util/Macros.hpp>

MethodHandlingFilter::MethodHandlingFilter()
{
}

MethodHandlingFilter::MethodHandlingFilter(const MethodHandlingFilter &other)
{
	(void)other;
}

MethodHandlingFilter::~MethodHandlingFilter()
{
}

MethodHandlingFilter&
MethodHandlingFilter::operator=(const MethodHandlingFilter &other)
{
	(void)other;

	return (*this);
}

void
MethodHandlingFilter::doFilter(UNUSED HTTPClient &client, UNUSED Request &request, UNUSED Response &response, FilterChain &next)
{
	return (next());
}
