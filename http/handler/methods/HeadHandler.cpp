/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:51:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:51:44 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Configuration.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/HeadHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <io/File.hpp>
#include <util/URL.hpp>
#include <list>
#include <string>

HeadHandler::HeadHandler()
{
}

HeadHandler::~HeadHandler()
{
}

HTTPResponse*
HeadHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;

	const std::string &path = request.root() + request.url().path();
	File file(path);

	if (!file.exists())
		return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));

	if (file.isFile())
	{
		headers.contentLength(file.length());

		std::string extension;
		if (request.url().extension(extension))
			headers.contentType(request.configuration().mimeRegistry(), extension);

		return (status(*HTTPStatus::OK, headers));
	}

	if (file.isDirectory())
	{
		const std::string &directory = request.url().path();

		std::string listing = ""
				"<html>\n"
				"	<head>\n"
				"		<title>Listing of " + directory + "</title>\n"
				"	</head>\n"
				"	<body>\n";

		std::list<File> files = file.list();
		for (std::list<File>::iterator it = files.begin(); it != files.end(); it++)
		{
			std::string name(it->name());

			if (it->isDirectory())
				name += '/';

			std::string path(request.url().path());
			if (path.empty() || path.at(path.size() - 1) != '/') // TODO Need rework!
				path += '/';

			path += name;

			listing += std::string("		<a href=\"") + path + "\">" + name + "</a><br>\n";
		}

		listing += ""
				"	</body>\n"
				"</html>\n";

		headers.html();
		headers.contentLength(listing.size());

		return (status(*HTTPStatus::OK, headers));
	}

	return (status(*HTTPStatus::NOT_FOUND));
}

HeadHandler&
HeadHandler::get(void)
{
	static HeadHandler handler;
	return (handler);
}
