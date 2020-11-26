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
#include <http/HTTPRequest.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>

ConnectHandler::ConnectHandler()
{
}

ConnectHandler::~ConnectHandler()
{
}

HTTPResponse*
ConnectHandler::handle(HTTPRequest &request)
{
	return (HTTPResponse::status(*HTTPStatus::BAD_REQUEST));
}
