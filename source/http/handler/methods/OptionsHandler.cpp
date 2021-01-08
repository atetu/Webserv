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

#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/OptionsHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>

OptionsHandler::OptionsHandler()
{
}

OptionsHandler::OptionsHandler(const OptionsHandler &other)
{
	(void)other;
}

OptionsHandler::~OptionsHandler()
{
}

OptionsHandler&
OptionsHandler::operator =(const OptionsHandler &other)
{
	(void)other;

	return (*this);
}

void
OptionsHandler::handle(HTTPRequest &request, HTTPResponse &response)
{
	response.headers().allow(request.allowedMethods());
	response.status(*HTTPStatus::OK);
}
