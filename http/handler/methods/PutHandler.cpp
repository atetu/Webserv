/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:43:22 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:43:22 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Configuration.hpp>
#include <exception/Exception.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/PutHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/mime/Mime.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/URL.hpp>
#include <io/FileDescriptor.hpp>
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <sys/fcntl.h>

static Logger &LOG = LoggerFactory::get("PUT");

PutHandler::PutHandler()
{
}

PutHandler::~PutHandler()
{
}

int
PutHandler::checkExtension(HTTPRequest &request, File &file)
{
	std::string type;

	std::map<std::string, std::string>::const_iterator it = request.headers().storage().find("Content-Type");
	if (it == request.headers().storage().end())
		type = "";
	else
		type = it->second;

	if (!type.empty())
	{
		const Mime *mime = request.configuration().mimeRegistry().findByMimeType(type);
		if (mime == NULL)
		{
			LOG.warn() << "Extension conversion not handled (1)" << std::endl;
			return (0);
		}

		std::string path = file.path();
		std::size_t found = path.find_last_of('.');
		std::string fileExtension;

		if (path[found + 1] != '/')
		{
			fileExtension = path.substr(found + 1);

			if (!fileExtension.empty())
			{
				Mime::iterator ext_it = std::find(mime->extensions().begin(), mime->extensions().end(), fileExtension);
				if (ext_it != mime->extensions().end())
					; // TODO Bad conditional
				else if (ext_it == mime->extensions().end() && file.exists())
				{
					LOG.warn() << "Extension conversion not handled(2)" << std::endl;
					return (0);
				}
				else
				{
					file = File(path.substr(0, found) + "." + *(mime->extensions().begin()));
				}
			}
		}
		else if (!file.exists())
			file = File(path + "." + *(mime->extensions().begin()));
	}
	return (1);
}

HTTPResponse*
PutHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;

	const std::string &path = request.root() + request.url().path();
	//File file(path + "/" + request.getLocation());
	File file(path);
	if (!checkExtension(request, file))
		return (GenericHTTPResponse::status(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE));

	if (!file.exists())
	{
		try
		{
			file.create();
		}
		catch (Exception &exception)
		{
			LOG.warn() << exception.what() << std::endl;
			return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
		}
	}
	if (file.isFile())
	{
		std::string created = "Ressource created";
		headers.contentLength(created.size());
		return (HTTPMethodHandler::filePut(*HTTPStatus::OK, *file.open(O_CREAT|O_WRONLY), request.body(), created, headers));
	}

	if (file.isDirectory())
	{
		LOG.warn() << "Put method not handled for directories" << std::endl;
		return (GenericHTTPResponse::status(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE));
	}

	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
}

PutHandler&
PutHandler::get(void)
{
	static PutHandler handler;

	return (handler);
}
