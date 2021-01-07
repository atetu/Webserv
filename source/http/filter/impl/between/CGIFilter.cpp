/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIFilter.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 00:27:23 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 00:27:23 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/between/CGIFilter.hpp>
#include <util/Macros.hpp>

CGIFilter::CGIFilter()
{
}

CGIFilter::CGIFilter(const CGIFilter &other)
{
	(void)other;
}

CGIFilter::~CGIFilter()
{
}

CGIFilter&
CGIFilter::operator=(const CGIFilter &other)
{
	(void)other;

	return (*this);
}

void
CGIFilter::doFilter(UNUSED HTTPClient &client, UNUSED Request &request, UNUSED Response &response, FilterChain &next)
{
	return (next());
}
