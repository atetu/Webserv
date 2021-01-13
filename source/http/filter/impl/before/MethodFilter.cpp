/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodFilter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 19:16:46 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/12 15:10:10 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/ServerBlock.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/before/MethodFilter.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
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
MethodFilter::doFilter(UNUSED HTTPClient &client, HTTPRequest &request, HTTPResponse &response, FilterChain &next)
{
	const HTTPMethod *methodPtr = HTTPMethod::find(client.parser().method());
	if (methodPtr)
	{
		const HTTPMethod &method = *methodPtr;
		request.method(method);

		if (isAcceptable(request.serverBlock(), request.locationBlock(), method))
			return (next());
	}

	response.headers().allow(request.allowedMethods());
	response.status(*HTTPStatus::METHOD_NOT_ALLOWED);
	response.end();
}

bool
MethodFilter::isAcceptable(const Optional<const ServerBlock*> &serverBlock, const Optional<const LocationBlock*> &locationBlock, const HTTPMethod &method)
{
	if (locationBlock.present() && (*locationBlock.get()).hasMethod(method.name()))
		return (true);

	if (serverBlock.present() && (*serverBlock.get()).hasMethod(method.name()))
		return (true);

	return (false /* TODO: For now */);
}
