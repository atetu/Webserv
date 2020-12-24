/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethodHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 20:35:21 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 20:35:21 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/handler/HTTPMethodHandler.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPRequest.hpp>
#include <http/page/DefaultPages.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <util/Singleton.hpp>

HTTPMethodHandler::~HTTPMethodHandler()
{
}

GenericHTTPResponse*
HTTPMethodHandler::status(HTTPStatus &status)
{
	return (GenericHTTPResponse::string(status, HTTPHeaderFields(), DefaultPages::instance().resolve(status)));
}

GenericHTTPResponse*
HTTPMethodHandler::error(const HTTPRequest &request, HTTPStatus &httpStatus)
{
	(void)request; // TODO Need to handle custom error pages if necessary
	return (status(httpStatus));
}
