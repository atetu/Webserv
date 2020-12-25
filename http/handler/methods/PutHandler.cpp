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

#include <exception/Exception.hpp>
#include <http/handler/methods/PutHandler.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPStatus.hpp>
#include <http/mime/Mime.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/URL.hpp>
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <list>
#include <map>
#include <string>

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
		const Mime *mime = request.mimeRegistry().findByMimeType(type);
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
					;
				else if (ext_it == mime->extensions().end() && file.exists())
				{
					LOG.warn() << "Extension conversion not handled(2)" << std::endl;
					return (0);
				}
				else
				{
					file.setNewPath(path.substr(0, found), *(mime->extensions().begin()));
				}
			}
		}
		else if (!file.exists())
			file.setNewPath(path, *(mime->extensions().begin()));
	}
	return (1);
}

HTTPResponse*
PutHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;

	const std::string &path = request.root() + request.url().path();
	File file(path);

	if (!checkExtension(request, file))
		return (GenericHTTPResponse::status(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE));

	if (!file.exists())
	{
		try
		{
			file.create(request.getLocation());
		}
		catch (Exception &exception)
		{
			LOG.warn() << exception.what() << std::endl;
			return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
		}
	}
	if (file.isFile())
	{
		std::ofstream stream;
		stream.open(file.path().c_str(), std::ios::out | std::ios::app);
		stream << request.body(); // TODO Blocking IO is against subject
		stream.close();
		std::string created = "Ressource created";
		headers.contentLength(created.size());
		return (GenericHTTPResponse::string(*HTTPStatus::CREATED, headers, created));
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
