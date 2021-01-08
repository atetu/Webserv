/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:13:28 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:13:28 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/DeleteHandler.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <io/File.hpp>

DeleteHandler::DeleteHandler()
{
}

DeleteHandler::DeleteHandler(const DeleteHandler &other)
{
	(void)other;
}

DeleteHandler::~DeleteHandler()
{
}

DeleteHandler&
DeleteHandler::operator =(const DeleteHandler &other)
{
	(void)other;

	return (*this);
}

void
DeleteHandler::handle(HTTPRequest &request, HTTPResponse &response)
{
	File targetFile(request.targetFile());

	if (!targetFile.exists())
		return (response.status(*HTTPStatus::NO_CONTENT));

	if (targetFile.tryRemove())
		return (response.status(*HTTPStatus::OK));
	else
		return (response.status(*HTTPStatus::ACCEPTED));
}
