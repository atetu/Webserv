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
#include <sys/stat.h>
#include <dirent.h>
#include <http/handler/methods/GetHandler.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>
#include <io/FileDescriptor.hpp>
#include <sys/fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <util/buffer/impl/FileBuffer.hpp>
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

	const std::string &path = request.root() + request.url().path();

	struct stat st;
	if (::stat(path.c_str(), &st) != 0)
		return (HTTPResponse::status(*HTTPStatus::NOT_FOUND));

	if (S_ISREG(st.st_mode))
	{
		int fd = ::open(path.c_str(), O_RDONLY);

		if (fd == -1)
			return (HTTPResponse::status(*HTTPStatus::NOT_FOUND));

		std::string extension;
		if (request.url().extension(extension))
			headers.contentType(request.configuration().mimeRegistry(), extension);

		headers.contentLength(st.st_size);

		return (new HTTPResponse(*HTTPStatus::OK, headers, new HTTPResponse::FileBody(*FileBuffer::from(*FileDescriptor::wrap(fd)))));
	}

	if (S_ISDIR(st.st_mode))
	{
		DIR *dir = ::opendir(path.c_str());

		if (dir == NULL)
			return (HTTPResponse::status(*HTTPStatus::NOT_FOUND));

		const std::string &directory = request.url().path();

		std::string listing = ""
				"<html>\n"
				"	<head>\n"
				"		<title>Listing of " + directory + "</title>\n"
				"	</head>\n"
				"	<body>\n";

		struct dirent *entry;
		while ((entry = ::readdir(dir)))
		{
			std::string file(entry->d_name);
			std::string absolute = directory + "/" + file;

//			if (::stat(absolute.c_str(), &st) != -1 && S_ISDIR(st.st_mode))
//				file += '/';

			listing += std::string("		<a href=\"./") + absolute + "\">" + file + "</a><br>\n";
		}

		listing += ""
				"	</body>\n"
				"</html>\n";

		::closedir(dir);

		headers.html();
		headers.contentLength(listing.size());

		return (new HTTPResponse(*HTTPStatus::OK, headers, new HTTPResponse::StringBody(listing)));
	}

	return (HTTPResponse::status(*HTTPStatus::NOT_FOUND));
}

GetHandler&
GetHandler::get(void)
{
	static GetHandler handler;

	return (handler);
}
