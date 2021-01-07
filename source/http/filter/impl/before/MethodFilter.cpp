/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodFilter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 19:16:46 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 19:16:46 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/ServerBlock.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/before/MethodFilter.hpp>
#include <http/filter/x/Request.hpp>
#include <http/filter/x/Response.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <util/Enum.hpp>
#include <util/Macros.hpp>

MethodFilter::MethodFilter()
{
}

MethodFilter::MethodFilter(const MethodFilter &other)
{
	(void)other;
}

MethodFilter::~MethodFilter()
{
}

MethodFilter&
MethodFilter::operator=(const MethodFilter &other)
{
	(void)other;

	return (*this);
}

void
MethodFilter::doFilter(UNUSED HTTPClient &client, Request &request, Response &response, FilterChain &next)
{
	const HTTPMethod *methodPtr = HTTPMethod::find(client.parser().method());
	if (methodPtr)
	{
		const HTTPMethod &method = *methodPtr;

		if (isAcceptable(request.serverBlock(), request.locationBlock(), method))
		{
			request.method(method);

			return (next());
		}
	}

	response.headers().allow(request.allowedMethods());
	response.headers().allow(HTTPMethod::values());
	response.status(*HTTPStatus::METHOD_NOT_ALLOWED);
}

bool
MethodFilter::isAcceptable(const Optional<const ServerBlock*> &serverBlock, const Optional<const LocationBlock*> &locationBlock, const HTTPMethod &method)
{
	if (locationBlock.present() && (*locationBlock.get()).hasMethod(method.name()))
		return (true);

	if (serverBlock.present() && (*serverBlock.get()).hasMethod(method.name()))
		return (true);

	return (true /* TODO: For now */);
}
