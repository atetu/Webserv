/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadFilter.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 00:35:58 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 00:35:58 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/enums/HTTPMethod.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/after/HeadFilter.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <stddef.h>
#include <util/Macros.hpp>
#include <util/Optional.hpp>

HeadFilter::HeadFilter()
{
}

HeadFilter::HeadFilter(const HeadFilter &other)
{
	(void)other;
}

HeadFilter::~HeadFilter()
{
}

HeadFilter&
HeadFilter::operator=(const HeadFilter &other)
{
	(void)other;

	return (*this);
}

void
HeadFilter::doFilter(UNUSED HTTPClient &client, UNUSED HTTPRequest &request, HTTPResponse &response, FilterChain &next)
{
	if (request.method().present() && request.method().get() == HTTPMethod::HEAD)
		response.body(NULL);

	return (next());
}
