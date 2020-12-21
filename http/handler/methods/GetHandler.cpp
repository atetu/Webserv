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
#include <http/HTTPStatus.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <io/File.hpp>
#include <io/FileDescriptor.hpp>
#include <stddef.h>
#include <sys/fcntl.h>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <util/URL.hpp>
#include <list>
#include <string>

GetHandler::GetHandler()
{
}

GetHandler::~GetHandler()
{
}

GenericHTTPResponse*
GetHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;

	const std::string &path = request.root() + request.url().path();
	File file(path);

	if (!file.exists())
		return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));

	if (file.isFile())
	{
		size_t length = file.length(); /* On top because it can cause an IOException. */

		int fd = ::open(path.c_str(), O_RDONLY); // TODO Need abstraction

		if (fd == -1)
			return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));

		std::string extension;
		if (request.url().extension(extension))
			headers.contentType(request.configuration().mimeRegistry(), extension);

		headers.contentLength(file.length());

		return (GenericHTTPResponse::file(*HTTPStatus::OK, headers, *FileDescriptorBuffer::from(*FileDescriptor::wrap(fd), FileDescriptorBuffer::CLOSE | FileDescriptorBuffer::DELETE)));
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

		return (GenericHTTPResponse::string(*HTTPStatus::OK, headers, listing));
	}

	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
}

GetHandler&
GetHandler::get(void)
{
	static GetHandler handler;

	return (handler);
}
