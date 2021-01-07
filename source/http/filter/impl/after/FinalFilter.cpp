/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FinalFilter.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 01:19:41 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 01:19:41 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/after/FinalFilter.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <util/Macros.hpp>
#include <util/Optional.hpp>
#include <util/StringUtils.hpp>
#include <string>

FinalFilter::FinalFilter()
{
}

FinalFilter::FinalFilter(const FinalFilter &other)
{
	(void)other;
}

FinalFilter::~FinalFilter()
{
}

FinalFilter&
FinalFilter::operator=(const FinalFilter &other)
{
	(void)other;

	return (*this);
}

void
FinalFilter::doFilter(UNUSED HTTPClient &client, UNUSED HTTPRequest &request, HTTPResponse &response, FilterChain &next)
{
	{
		static std::string keepAlive = "keep-alive";
		static std::string close = "close";

		Optional<std::string> connectionOpt = request.headers().get(HTTPHeaderFields::CONNECTION);

		if (connectionOpt.present() && StringUtils::equalsIgnoreCase(connectionOpt.get(), keepAlive))
			response.headers().connection(close);
	}

	response.headers().date();
	response.headers().server();
	response.end();

	return (next());
}
