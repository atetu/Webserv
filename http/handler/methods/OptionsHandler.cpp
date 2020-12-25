/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionsHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:42:58 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:42:58 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <http/handler/methods/OptionsHandler.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPStatus.hpp>
#include <util/Convert.hpp>
#include <util/Optional.hpp>
#include <list>
#include <string>

OptionsHandler::OptionsHandler()
{
}

OptionsHandler::~OptionsHandler()
{
}

HTTPResponse*
OptionsHandler::handle(HTTPRequest &request)
{
	static std::string comaAndSpace = ", ";

	std::string allowValue;

	if (request.location().present() && request.location().get()->methods().present())
		allowValue = Convert::join(request.location().get()->methods().get(), comaAndSpace);
	else
		allowValue = "GET, PUT, POST, CONNECT, DELETE, HEAD, TRACE, OPTIONS";

	HTTPHeaderFields headers;
	headers.contentLength(0);
	headers.allow(allowValue);

	return (status(*HTTPStatus::CREATED, headers));
}

OptionsHandler&
OptionsHandler::get(void)
{
	static OptionsHandler handler;

	return (handler);
}
