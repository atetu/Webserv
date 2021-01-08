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

#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/PutHandler.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>

PutHandler::PutHandler()
{
}

PutHandler::PutHandler(const PutHandler &other)
{
	(void)other;
}

PutHandler::~PutHandler()
{
}

PutHandler&
PutHandler::operator =(const PutHandler &other)
{
//	HTTPHeaderFields headers;
//
//std::cout << "put : "<< request.body().size() << std::endl;
//	const std::string &path = request.root() + request.url().path();
//	//File file(path + "/" + request.getLocation());
//	File file(path);
//	if (!checkExtension(request, file))
//	//	return (status(*HTTPStatus::CONFLICT, headers));
//	//	return (error(request, *HTTPStatus::BAD_REQUEST));
//		return (statusEmpty(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE, headers));
//
//	if (!file.exists())
//	{
//		try
//		{
//		//	std::cout << "create\n";
//			file.createNewFile(0777);
//		}
//		catch (Exception &exception)
//		{
//			LOG.warn() << exception.what() << std::endl;
//		//	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
//			return (error(request, *HTTPStatus::NOT_FOUND));
//		}
//	}
//	if (file.isFile())
//	{
//		std::string created = "Ressource created";
//		headers.contentLength(created.size());
//		if (request.method().name() == "POST")
//		{
//			return (HTTPMethodHandler::filePut(*HTTPStatus::OK, *file.open(O_WRONLY|O_APPEND), request.body(), created, headers));
//		}
//		else if (request.method().name() == "PUT")
//			return (HTTPMethodHandler::filePut(*HTTPStatus::OK, *file.open(O_WRONLY), request.body(), created, headers));
//	}
//
//	if (file.isDirectory())
//	{
//		return (error(request, *HTTPStatus::METHOD_NOT_ALLOWED));
//		return (statusEmpty(*HTTPStatus::OK, headers));
//		//return (HTTPMethodHandler::filePut(*HTTPStatus::OK, *file.open(O_WRONLY|O_APPEND), request.body(), created, headers));
//		LOG.warn() << "Put method not handled for directories" << std::endl;
//		return (error(request, *HTTPStatus::METHOD_NOT_ALLOWED));
//	//	return (statusEmpty(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE, headers));
//	//	return (GenericHTTPResponse::status(*HTTPStatus::UNSUPPORTED_MEDIA_TYPE));
//	}
//
//	return (error(request, *HTTPStatus::NOT_FOUND));
////	return (GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND));
	(void)other;

	return (*this);
}

void
PutHandler::handle(HTTPRequest &request, HTTPResponse &response)
{
	(void)request;

	response.status(*HTTPStatus::DESTINATION_LOCKED);
}
