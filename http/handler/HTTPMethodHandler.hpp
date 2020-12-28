/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 17:59:33 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 17:59:33 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHODHANDLER_HPP_
# define HTTPMETHODHANDLER_HPP_

#include <http/header/HTTPHeaderFields.hpp>
#include <io/FileDescriptor.hpp>
#include <string>

class HTTPRequest;
class HTTPResponse;
class HTTPStatus;

class HTTPMethodHandler
{
	public:
		virtual
		~HTTPMethodHandler();

		virtual HTTPResponse*
		handle(HTTPRequest &request) = 0;

	public:
		static HTTPResponse*
		status(HTTPStatus &status, const HTTPHeaderFields &headers = HTTPHeaderFields());

		static HTTPResponse*
		file(HTTPStatus &httpStatus, int fd, const HTTPHeaderFields &headers = HTTPHeaderFields());

		static HTTPResponse*
		file(HTTPStatus &httpStatus, FileDescriptor &fileDescriptor, const HTTPHeaderFields &headers = HTTPHeaderFields());

		static HTTPResponse*
		string(HTTPStatus &httpStatus, const std::string &string, const HTTPHeaderFields &headers = HTTPHeaderFields());

		static HTTPResponse*
		filePut(HTTPStatus &httpStatus, FileDescriptor &fileDescriptor, const std::string &body, const std::string &string, const HTTPHeaderFields &headers);

		static HTTPResponse*
		error(const HTTPRequest &request, HTTPStatus &httpStatus, const HTTPHeaderFields &headers = HTTPHeaderFields());
};

#endif /* HTTPMETHODHANDLER_HPP_ */
