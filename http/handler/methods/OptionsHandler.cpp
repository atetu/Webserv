/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionsHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:42:58 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:42:58 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Configuration.hpp>
#include <http/handler/methods/OptionsHandler.hpp>
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


static Logger &LOG = LoggerFactory::get("Options");

OptionsHandler::OptionsHandler()
{
}

OptionsHandler::~OptionsHandler()
{
}

GenericHTTPResponse*
OptionsHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;
	std::string allowValue;
	int present = 0;

	if (request.location().present())
	{
		if (request.location().get()->methods().present())
		{
			present = 1;
			std::list<std::string> methodList = request.location().get()->methods().get();
			std::list<std::string>::iterator it = methodList.begin();
			std::list<std::string>::iterator ite = methodList.end();
			while (it != ite)
			{
				allowValue += *it;
				it++;
				if (it != ite)
					allowValue += ", ";
			}
		}	
	}
	
	if (!present)
		allowValue = "GET, PUT, POST, CONNECT, DELETE, HEAD, TRACE, OPTIONS";

	headers.contentLength(0);
	headers.allow(allowValue);
	
	return (GenericHTTPResponse::noBody(*HTTPStatus::CREATED, headers));
}

OptionsHandler&
OptionsHandler::get(void)
{
	static OptionsHandler handler;

	return (handler);
}
