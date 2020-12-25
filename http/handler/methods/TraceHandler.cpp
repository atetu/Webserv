/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TraceHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:42:58 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:42:58 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/enums/HTTPStatus.hpp>
#include <http/handler/methods/TraceHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/request/HTTPRequest.hpp>
#include <string>

TraceHandler::TraceHandler()
{
}

TraceHandler::~TraceHandler()
{
}

HTTPResponse*
TraceHandler::handle(HTTPRequest &request)
{
	HTTPHeaderFields headers;

#if 0 // TODO The 'status line' variable is never used
	std::string method = request.method().name();
	std::string host = request.url().host();
	std::string path = request.url().path(); // TODO check queries and fragments with Nginx
	std::string version = "HTTTP/1.1";
	std::string statusLine = method + ' ' + host + path + ' ' + version + '\n';
#endif

	std::string body = request.headers().format("\n");

	headers.httpMessage();
	headers.contentLength(body.size());

	return (string(*HTTPStatus::OK, body, headers));
}

TraceHandler&
TraceHandler::get(void)
{
	static TraceHandler handler;

	return (handler);
}
