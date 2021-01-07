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
#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <util/Singleton.hpp>
#include <util/URL.hpp>

HTTPMethodHandler::~HTTPMethodHandler()
{
}

HTTPResponse*
HTTPMethodHandler::status(const HTTPStatus &status, const HTTPHeaderFields &headers)
{
	return (GenericHTTPResponse::string(status, HTTPHeaderFields(headers).html(), DefaultPages::instance().resolve(status)));
}

HTTPResponse*
HTTPMethodHandler::statusHead(const HTTPStatus &status, const HTTPHeaderFields &headers)
{
	const std::string &defaultPage = DefaultPages::instance().resolve(status);
	HTTPHeaderFields(headers).contentLength(defaultPage.size());
	//(defaultPage.size());
	const std::string empty = "";
	return (GenericHTTPResponse::string(status, HTTPHeaderFields(headers).html(), empty));
}

HTTPResponse*
HTTPMethodHandler::statusEmpty(const HTTPStatus &status, const HTTPHeaderFields &headers)
{
	//return (GenericHTTPResponse::string(status, headers, DefaultPages::instance().resolve(status)));
	return (GenericHTTPResponse::string(status, headers, ""));
}

HTTPResponse*
HTTPMethodHandler::file(const HTTPStatus &httpStatus, int fd, const HTTPHeaderFields &headers)
{
	return (file(httpStatus, *FileDescriptor::wrap(fd), headers));
}

HTTPResponse*
HTTPMethodHandler::file(const HTTPStatus &httpStatus, FileDescriptor &fileDescriptor, const HTTPHeaderFields &headers)
{
	return (GenericHTTPResponse::file(httpStatus, headers, *FileDescriptorBuffer::from(fileDescriptor, FileDescriptorBuffer::CLOSE | FileDescriptorBuffer::DELETE)));
}

HTTPResponse*
HTTPMethodHandler::string(const HTTPStatus &httpStatus, const std::string &string, const HTTPHeaderFields &headers)
{
	return (GenericHTTPResponse::string(httpStatus, headers, string));
}

HTTPResponse*
HTTPMethodHandler::filePut(const HTTPStatus &httpStatus, FileDescriptor &fileDescriptor, const std::string &body, const std::string &string, const HTTPHeaderFields &headers)
{
	FileDescriptorBuffer *buffer = FileDescriptorBuffer::from(fileDescriptor, FileDescriptorBuffer::CLOSE | FileDescriptorBuffer::DELETE);
	buffer->store(body);
	return (GenericHTTPResponse::fileAndString(httpStatus, headers, *buffer, string));
}

HTTPResponse*
HTTPMethodHandler::error(const HTTPRequest &request, const HTTPStatus &httpStatus, const HTTPHeaderFields &headers)
{
	(void)request; // TODO Need to handle custom error pages if necessary
	return (status(httpStatus, headers));
}

HTTPResponse*
HTTPMethodHandler::errorHead(const HTTPRequest &request, const HTTPStatus &httpStatus, const HTTPHeaderFields &headers)
{
	(void)request; // TODO Need to handle custom error pages if necessary
	return (statusHead(httpStatus, headers));
}

HTTPResponse*
HTTPMethodHandler::redirect(const HTTPStatus &httpStatus, const URL &to, const HTTPHeaderFields &headers)
{
	return (status(httpStatus, HTTPHeaderFields(headers).location(to)));
}
