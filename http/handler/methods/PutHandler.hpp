/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:51:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:51:44 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUTHANDLER_HPP_
# define PUTHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>
#include <http/HTTPRequest.hpp>
#include <io/File.hpp>

class PutHandler :
		public HTTPMethodHandler
{
	public:
		PutHandler();

		virtual
		~PutHandler();

		GenericHTTPResponse*
		handle(HTTPRequest &request);

		int
		checkExtension(HTTPRequest &request, File &file);

	public:
		static PutHandler&
		get(void);
};

#endif /* PutHANDLER_HPP_ */
