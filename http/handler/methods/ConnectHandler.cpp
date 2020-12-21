/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CONNECT.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:02:05 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:02:05 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/handler/methods/ConnectHandler.hpp>
#include <http/HTTPStatus.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>

ConnectHandler::ConnectHandler()
{
}

ConnectHandler::~ConnectHandler()
{
}

GenericHTTPResponse*
ConnectHandler::handle(HTTPRequest &request)
{
	return (GenericHTTPResponse::status(*HTTPStatus::BAD_REQUEST));
}

ConnectHandler&
ConnectHandler::get(void)
{
	static ConnectHandler handler;

	return (handler);
}
