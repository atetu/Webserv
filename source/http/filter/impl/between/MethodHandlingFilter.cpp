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

#include <exception/Exception.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/between/MethodHandlingFilter.hpp>
#include <http/handler/HTTPMethodHandler.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <log/LoggerFactory.hpp>
#include <util/Macros.hpp>
#include <util/Optional.hpp>

Logger &MethodHandlingFilter::LOG = LoggerFactory::get("Meth. Handl. Filter");

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
	if (response.status().absent())
	{
		try
		{
			request.method().get()->handler().handle(request, response);
		}
		catch (Exception &exception)
		{
			LOG.trace() << "Failed to handle method: " << exception.message() << std::endl;

			response.status(*HTTPStatus::INTERNAL_SERVER_ERROR);
		}
	}

	return (next());
}
