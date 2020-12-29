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
#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/GetHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/request/HTTPRequest.hpp>
#include <io/File.hpp>
#include <io/FileDescriptor.hpp>
#include <sys/fcntl.h>
#include <util/StringUtils.hpp>
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

	File targetFile(request.root() + request.url().path());

	if (!targetFile.exists())
		return (error(request, *HTTPStatus::NOT_FOUND));

	if (targetFile.isFile())
	{
		headers.contentLength(targetFile.length());

		std::string extension;
		if (request.url().extension(extension))
			headers.contentType(request.configuration().mimeRegistry(), extension);

		return (file(*HTTPStatus::OK, *targetFile.open(O_RDONLY), headers));
	}

	if (targetFile.isDirectory())
	{
		if (StringUtils::last(request.url().path()) != '/')
			return (redirect(*HTTPStatus::MOVED_PERMANENTLY, request.url().builder().appendToPath("/").build()));

		std::string content = listing(request.url(), targetFile);

		headers.html();
		headers.contentLength(content.size());

		return (string(*HTTPStatus::OK, content, headers));
	}

	return (error(request, *HTTPStatus::NOT_FOUND));
}

std::string
GetHandler::listing(const URL &url, const File &file)
{
	const std::string &directory = url.path();

	std::string out = ""
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

		std::string path(url.path());
		if (path.empty() || path.at(path.size() - 1) != '/') // TODO Need rework!
			path += '/';

		path += name;

		out += std::string("		<a href=\"") + path + "\">" + name + "</a><br>\n";
	}

	out += ""
			"	</body>\n"
			"</html>\n";

	return (out);
}

GetHandler&
GetHandler::get(void)
{
	static GetHandler handler;

	return (handler);
}
