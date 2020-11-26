/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:51:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:51:44 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cygwin/stat.h>
#include <http/handler/methods/GetHandler.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>
#include <sys/errno.h>
#include <sys/stat.h>
#include <util/URL.hpp>
#include <string>

GetHandler::GetHandler()
{
}

GetHandler::~GetHandler()
{
}

HTTPResponse*
GetHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;

	const std::string &path = request.url().path();

	struct stat st;
	if (::stat(path.c_str(), &st) != 0)
		return (HTTPResponse::status(*HTTPStatus::NOT_FOUND));

	if (S_ISREG(st.st_mode))
	{
//		headers.contentType(registry, extension)
	}

	if (S_ISDIR(st.st_mode))
	{
	}

	return (HTTPResponse::status(*HTTPStatus::OK));
}
