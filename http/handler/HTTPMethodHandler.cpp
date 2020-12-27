/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethodHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 20:35:21 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 20:35:21 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/handler/HTTPMethodHandler.hpp>
#include <http/page/DefaultPages.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <util/Singleton.hpp>

HTTPMethodHandler::~HTTPMethodHandler()
{
}

HTTPResponse*
HTTPMethodHandler::status(HTTPStatus &status, const HTTPHeaderFields &headers)
{
	return (GenericHTTPResponse::string(status, HTTPHeaderFields(), DefaultPages::instance().resolve(status)));
}

HTTPResponse*
HTTPMethodHandler::file(HTTPStatus &httpStatus, int fd, const HTTPHeaderFields &headers)
{
	return (file(httpStatus, *FileDescriptor::wrap(fd), headers));
}

HTTPResponse*
HTTPMethodHandler::file(HTTPStatus &httpStatus, FileDescriptor &fileDescriptor, const HTTPHeaderFields &headers)
{
	return (GenericHTTPResponse::file(httpStatus, headers, *FileDescriptorBuffer::from(fileDescriptor, FileDescriptorBuffer::CLOSE | FileDescriptorBuffer::DELETE)));
}

HTTPResponse*
HTTPMethodHandler::string(HTTPStatus &httpStatus, const std::string &string, const HTTPHeaderFields &headers)
{
	return (GenericHTTPResponse::string(httpStatus, headers, string));
}

HTTPResponse*
HTTPMethodHandler::filePut(HTTPStatus &httpStatus, FileDescriptor &fileDescriptor, const std::string &body, const std::string &string, const HTTPHeaderFields &headers)
{
	FileDescriptorBuffer *buffer = FileDescriptorBuffer::from(fileDescriptor, FileDescriptorBuffer::CLOSE | FileDescriptorBuffer::DELETE);
	buffer->store(body);
	return (GenericHTTPResponse::fileAndString(httpStatus, headers, *buffer, string));
}

HTTPResponse*
HTTPMethodHandler::error(const HTTPRequest &request, HTTPStatus &httpStatus, const HTTPHeaderFields &headers)
{
	(void)request; // TODO Need to handle custom error pages if necessary
	return (status(httpStatus, headers));
}
