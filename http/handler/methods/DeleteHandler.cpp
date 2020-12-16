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

#include <http/handler/methods/DeleteHandler.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>
#include <sys/stat.h>
#include <cstdio>
#include <string>

DeleteHandler::DeleteHandler()
{
}

DeleteHandler::~DeleteHandler()
{
}

HTTPResponse*
DeleteHandler::handle(HTTPRequest &request)
{
	const char *filepath = request.url().path().c_str();
	struct stat st;

	if (::stat(filepath, &st) == 0)
	{
		if (::remove(filepath) == 0)
			return (HTTPResponse::status(*HTTPStatus::OK));
		else
			return (HTTPResponse::status(*HTTPStatus::ACCEPTED));
	}
	else
		return (HTTPResponse::status(*HTTPStatus::NO_CONTENT));
}

DeleteHandler&
DeleteHandler::get(void)
{
	static DeleteHandler handler;

	return (handler);
}
