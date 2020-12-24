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
#include <http/HTTPStatus.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <sys/stat.h>
#include <util/URL.hpp>
#include <cstdio>
#include <string>

DeleteHandler::DeleteHandler()
{
}

DeleteHandler::~DeleteHandler()
{
}

GenericHTTPResponse*
DeleteHandler::handle(HTTPRequest &request)
{
	const char *filepath = request.url().path().c_str();
	struct stat st;

	if (::stat(filepath, &st) == 0)
	{
		std::cout << filepath << std::endl;
		/*if (::remove(filepath) == 0) // TODO Disabled to avoid mistakes
			return (status(*HTTPStatus::OK));
		else
			return (status(*HTTPStatus::ACCEPTED));*/
		return (status(*HTTPStatus::OK));
	}
	else
		return (status(*HTTPStatus::NO_CONTENT));
}

DeleteHandler&
DeleteHandler::get(void)
{
	static DeleteHandler handler;

	return (handler);
}
