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

#include <config/Configuration.hpp>
#include <http/handler/methods/GetHandler.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPStatus.hpp>
#include <io/File.hpp>
#include <stddef.h>
#include <sys/fcntl.h>
#include <util/URL.hpp>
#include <list>
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

	const std::string &path = request.root() + request.url().path();
	File targetFile(path);

	if (!targetFile.exists())
		return (error(request, *HTTPStatus::NOT_FOUND));

	if (targetFile.isFile())
	{
		size_t length = targetFile.length(); /* On top because it can cause an IOException. */

		int fd = ::open(path.c_str(), O_RDONLY); // TODO Need abstraction

		if (fd == -1)
			return (error(request, *HTTPStatus::NOT_FOUND));

		std::string extension;
		if (request.url().extension(extension))
			headers.contentType(request.configuration().mimeRegistry(), extension);

		headers.contentLength(length);

		return (file(*HTTPStatus::OK, fd, headers));
	}

	if (targetFile.isDirectory())
	{
		const std::string &directory = request.url().path();

		std::string listing = ""
				"<html>\n"
				"	<head>\n"
				"		<title>Listing of " + directory + "</title>\n"
				"	</head>\n"
				"	<body>\n";

		std::list<File> files = targetFile.list();
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

		return (string(*HTTPStatus::OK, listing, headers));
	}

	return (error(request, *HTTPStatus::NOT_FOUND));
}

GetHandler&
GetHandler::get(void)
{
	static GetHandler handler;

	return (handler);
}
