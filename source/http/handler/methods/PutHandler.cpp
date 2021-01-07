/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:43:22 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:43:22 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/PutHandler.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>

PutHandler::PutHandler()
{
}

PutHandler::PutHandler(const PutHandler &other)
{
	(void)other;
}

PutHandler::~PutHandler()
{
}

PutHandler&
PutHandler::operator =(const PutHandler &other)
{
	(void)other;

	return (*this);
}

void
PutHandler::handle(HTTPRequest &request, HTTPResponse &response)
{
	(void)request;

	response.status(*HTTPStatus::DESTINATION_LOCKED);
}
