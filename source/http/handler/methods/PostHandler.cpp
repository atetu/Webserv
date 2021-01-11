/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:43:22 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:43:22 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/PostHandler.hpp>
#include <http/handler/methods/task/PutTask.hpp>
#include <http/HTTPClient.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <sys/fcntl.h>

PostHandler::PostHandler()
{
}

PostHandler::PostHandler(const PostHandler &other)
{
	(void)other;
}

PostHandler::~PostHandler()
{
}

PostHandler&
PostHandler::operator =(const PostHandler &other)
{
	(void)other;

	return (*this);
}

bool
PostHandler::handle(HTTPClient &client, HTTPRequest &request, HTTPResponse &response)
{
	File targetFile(request.targetFile());

	bool justCreated = false;
	
	std::string out;
	targetFile.findExtension(targetFile.path(), out);
	
	if (!targetFile.findMime(out, request))
	{
		response.status(*HTTPStatus::CONFLICT); // don't know the status 
		return (true);
	}

	if (targetFile.exists())
	{
		if (targetFile.isDirectory() || !targetFile.isFile())
		{
			response.status(*HTTPStatus::LOCKED);
			return (true);
		}
	else
	{
		try
		{
			targetFile.createNewFile(0777);
			response.headers().contentLocation(targetFile.path());
			justCreated = true;
		}
		catch (Exception &exception)
		{
			response.status(*HTTPStatus::INTERNAL_SERVER_ERROR); // don't know the status
			return (true);
	//		LOG.warn() << exception.what() << std::endl;
		//	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
		//	return (error(request, *HTTPStatus::NOT_FOUND));
		}
	}
	
	FileDescriptor &fd = *targetFile.open(O_CREAT | O_WRONLY | O_APPEND, 0664);

	client.task(*(new PutTask(client, fd, justCreated)));

	return (false);
}

// int
// PostHandler::checkExtension(HTTPRequest &request, File &file)
// {
// 	std::string type = request.headers().get(HTTPHeaderFields::CONTENT_TYPE).orElse("");

// 	if (!type.empty())
// 	{
// 		const Mime *mime = request.configuration().mimeRegistry().findByMimeType(type);
// 		if (mime == NULL)
// 		{
// 			//LOG.warn() << "Extension conversion not handled (1)" << std::endl;
// 			return (0);
// 		}

// 		std::string path = file.path();
// 		std::size_t found = path.find_last_of('.');
// 		std::string fileExtension;

// 		if (path[found + 1] != '/')
// 		{
// 			fileExtension = path.substr(found + 1);

// 			if (!fileExtension.empty())
// 			{
// 				Mime::iterator ext_it = std::find(mime->extensions().begin(), mime->extensions().end(), fileExtension);
// 				if (ext_it != mime->extensions().end())
// 					; // TODO Bad conditional
// 				else if (ext_it == mime->extensions().end() && file.exists())
// 				{
// 					LOG.warn() << "Extension conversion not handled(2)" << std::endl;
// 					return (0);
// 				}
// 				else
// 				{
// 					file = File(path.substr(0, found) + "." + *(mime->extensions().begin()));
// 				}
// 			}
// 		}
// 		else if (!file.exists())
// 			file = File(path + "." + *(mime->extensions().begin()));
// 	}

// 	return (1);
// }

// HTTPResponse*
// PostHandler::handle(HTTPRequest &request)
// {
// 	HTTPHeaderFields headers;

// 	const std::string &path = request.root() + request.url().path();
// 	//File file(path + "/" + request.getLocation());
// 	File file(path);
// 	if (!checkExtension(request, file))
// 	//	return (status(*HTTPStatus::CONFLICT, headers));
// 	//	return (error(request, *HTTPStatus::BAD_REQUEST));
// 		return (statusEmpty(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE, headers));

// 	if (!file.exists())
// 	{
// 		try
// 		{
// 		//	std::cout << "create\n";
// 			file.createNewFile(0777);
// 		}
// 		catch (Exception &exception)
// 		{
// 			LOG.warn() << exception.what() << std::endl;
// 		//	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
// 			return (error(request, *HTTPStatus::NOT_FOUND));
// 		}
// 	}
// 	if (file.isFile())
// 	{
// 		std::string created = "Ressource created";
// 		headers.contentLength(created.size());
// 		if (request.method().name() == "POST")
// 		{
// 			return (HTTPMethodHandler::filePost(*HTTPStatus::OK, *file.open(O_WRONLY|O_APPEND), request.body(), created, headers));
// 		}
// 		else if (request.method().name() == "Post")
// 			return (HTTPMethodHandler::filePost(*HTTPStatus::OK, *file.open(O_WRONLY), request.body(), created, headers));
// 	}

// 	if (file.isDirectory())
// 	{
// 		return (error(request, *HTTPStatus::METHOD_NOT_ALLOWED));
// 		return (statusEmpty(*HTTPStatus::OK, headers));
// 		//return (HTTPMethodHandler::filePost(*HTTPStatus::OK, *file.open(O_WRONLY|O_APPEND), request.body(), created, headers));
// 		LOG.warn() << "Post method not handled for directories" << std::endl;
// 		return (error(request, *HTTPStatus::METHOD_NOT_ALLOWED));
// 	//	return (statusEmpty(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE, headers));
// 	//	return (GenericHTTPResponse::status(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE));
// 	}

// 	return (error(request, *HTTPStatus::NOT_FOUND));
// //	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
// }
