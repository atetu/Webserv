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

class HTTPRequest;
class HTTPResponse;

class HTTPMethodHandler
{
	public:
		virtual
		~HTTPMethodHandler();

		virtual void
		handle(HTTPRequest &request, HTTPResponse &response) = 0;
};

#endif /* HTTPMETHODHANDLER_HPP_ */
