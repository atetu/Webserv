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
class HTTPResponse;
class HTTPStatus;

class HTTPRequest;
class GenericHTTPResponse;

class HTTPMethodHandler
{
	public:
		virtual
		~HTTPMethodHandler();

		virtual GenericHTTPResponse*
		handle(HTTPRequest &request) = 0;

	public:
		static GenericHTTPResponse*
		status(HTTPStatus &status);

		static GenericHTTPResponse*
		error(const HTTPRequest &request, HTTPStatus &httpStatus);
};

#endif /* HTTPMETHODHANDLER_HPP_ */
