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
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
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
	std::string type = request.headers().get(HTTPHeaderFields::CONTENT_TYPE).orElse("");

	if (!type.empty())
	{
		const Mime *mime = request.configuration().mimeRegistry().findByMimeType(type);
		if (mime == NULL)
		{
			//LOG.warn() << "Extension conversion not handled (1)" << std::endl;
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

std::cout << "put : "<< request.body().size() << std::endl;
	const std::string &path = request.root() + request.url().path();
	//File file(path + "/" + request.getLocation());
	File file(path);
	if (!checkExtension(request, file))
	//	return (status(*HTTPStatus::CONFLICT, headers));
	//	return (error(request, *HTTPStatus::BAD_REQUEST));
		return (statusEmpty(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE, headers));

	if (!file.exists())
	{
		try
		{
		//	std::cout << "create\n";
			file.createNewFile(0777);
		}
		catch (Exception &exception)
		{
			LOG.warn() << exception.what() << std::endl;
		//	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
			return (error(request, *HTTPStatus::NOT_FOUND));
		}
	}
	if (file.isFile())
	{
		std::string created = "Ressource created";
		headers.contentLength(created.size());
		if (request.method().name() == "POST")
		{
			return (HTTPMethodHandler::filePut(*HTTPStatus::OK, *file.open(O_WRONLY|O_APPEND), request.body(), created, headers));
		}
		else if (request.method().name() == "PUT")
			return (HTTPMethodHandler::filePut(*HTTPStatus::OK, *file.open(O_WRONLY), request.body(), created, headers));
	}

	if (file.isDirectory())
	{
		return (error(request, *HTTPStatus::METHOD_NOT_ALLOWED));
		return (statusEmpty(*HTTPStatus::OK, headers));
		//return (HTTPMethodHandler::filePut(*HTTPStatus::OK, *file.open(O_WRONLY|O_APPEND), request.body(), created, headers));
		LOG.warn() << "Put method not handled for directories" << std::endl;
		return (error(request, *HTTPStatus::METHOD_NOT_ALLOWED));
	//	return (statusEmpty(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE, headers));
	//	return (GenericHTTPResponse::status(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE));
	}

	return (error(request, *HTTPStatus::NOT_FOUND));
//	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
}

PutHandler&
PutHandler::get(void)
{
	static PutHandler handler;

	return (handler);
}
