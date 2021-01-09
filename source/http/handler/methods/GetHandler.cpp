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

#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <config/Configuration.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/GetHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/mime/Mime.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/body/impl/FileResponseBody.hpp>
#include <http/response/HTTPResponse.hpp>
#include <io/FileDescriptor.hpp>
#include <stddef.h>
#include <sys/fcntl.h>
#include <util/helper/DeleteHelper.hpp>
#include <util/Optional.hpp>
#include <util/Time.hpp>
#include <util/URL.hpp>
#include <iostream>
#include <list>

class FileDescriptorBuffer;

GetHandler::GetHandler()
{
}

GetHandler::GetHandler(const GetHandler &other)
{
	(void)other;
}

GetHandler::~GetHandler()
{
}

GetHandler&
GetHandler::operator =(const GetHandler &other)
{
	(void)other;

	return (*this);
}

void
GetHandler::handle(HTTPRequest &request, HTTPResponse &response)
{
	std::cout << request.targetFile().path() << std::endl;
	std::cout << request.targetFile().exists() << std::endl;

	File targetFile(request.targetFile());

	if (!targetFile.exists())
		return (response.status(*HTTPStatus::NOT_FOUND));

	if (targetFile.isFile())
	{
		size_t contentLength = targetFile.length();
		Time lastModified = targetFile.lastModified();
		Optional<const Mime*> contentType;

		std::string extension;
		if (request.url().extension(extension))
			contentType = Optional<const Mime*>::ofNullable(Configuration::instance().mimeRegistry().findByFileExtension(extension));

		FileDescriptor *fd = NULL;
		FileDescriptorBuffer *fdBuffer = NULL;

		try
		{
			fd = targetFile.open(O_RDONLY);
			fdBuffer = FileDescriptorBuffer::from(*fd, FileDescriptorBuffer::CLOSE | FileDescriptorBuffer::DELETE);

			response.body(new FileResponseBody(*fdBuffer));
			response.headers().contentLength(contentLength);
			response.headers().contentType(contentType);
			response.headers().lastModified(lastModified);
			response.status(*HTTPStatus::OK);
		}
		catch (...)
		{
			if (fdBuffer)
				DeleteHelper::pointer<FileDescriptorBuffer>(fdBuffer);
			else
				DeleteHelper::pointer<FileDescriptor>(fd); /* In case of memory allocation failing for the body. */

			throw;
		}

		return;
	}

	if (targetFile.isDirectory())
	{
		if (!request.listing())
			return (response.status(*HTTPStatus::FORBIDDEN));

		response.string(listing(request.url(), targetFile));
		response.headers().html();
		return (response.status(*HTTPStatus::OK));
	}

	return (response.status(*HTTPStatus::NOT_FOUND));
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
