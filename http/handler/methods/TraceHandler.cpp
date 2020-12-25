/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TraceHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:42:58 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:42:58 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Configuration.hpp>
#include <http/handler/methods/TraceHandler.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPStatus.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <io/File.hpp>
#include <io/FileDescriptor.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/log/LogLevel.hpp>
#include <stddef.h>
#include <sys/fcntl.h>
#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <http/mime/Mime.hpp>
#include <util/URL.hpp>
#include <list>
#include <string>
#include <iostream>
#include <fstream>


static Logger &LOG = LoggerFactory::get("Trace");

TraceHandler::TraceHandler()
{
}

TraceHandler::~TraceHandler()
{
}

GenericHTTPResponse*
TraceHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;

	std::string body;

	std::string method = request.method().name();
	std::string host = request.url().host();
	std::string path = request.url().path(); // TODO check queries and fragments with Nginx
	std::string version = "HTTTP/1.1";
	std::string statusLine = method + ' ' + host + path + ' ' + version + '\n';
	
	const HTTPHeaderFields::map & requestHeaders = request.headers().storage();
	std::map<std::string, std::string>::const_iterator it = requestHeaders.begin();
	std::map<std::string, std::string>::const_iterator ite = requestHeaders.end();
	while (it != ite)
	{
		body += it->first;
		body += ": ";
		body += it->second;
		it++;
		if (it != ite)
			body += '\n';

	}
	headers.contentType("message/http");
	headers.contentLength(body.size());
	std::cout << body << std::endl;
	return (GenericHTTPResponse::string(*HTTPStatus::OK, headers, body));
}

TraceHandler&
TraceHandler::get(void)
{
	static TraceHandler handler;

	return (handler);
}
