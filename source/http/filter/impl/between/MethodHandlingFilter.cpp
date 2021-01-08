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

#include <http/enums/HTTPMethod.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/between/MethodHandlingFilter.hpp>
#include <http/handler/HTTPMethodHandler.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <util/Macros.hpp>
#include <util/Optional.hpp>
#include <iostream>

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
MethodHandlingFilter::doFilter(UNUSED HTTPClient &client, UNUSED HTTPRequest &request, UNUSED HTTPResponse &response, FilterChain &next)
{
	request.method().get()->handler().handle(request, response);

	return (next());
}
