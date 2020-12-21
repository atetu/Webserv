/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:02:05 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:02:05 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTHANDLER_HPP_
# define CONNECTHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>

class ConnectHandler :
		public HTTPMethodHandler
{
	public:
		ConnectHandler();

		virtual
		~ConnectHandler();

		GenericHTTPResponse*
		handle(HTTPRequest &request);

	public:
		static ConnectHandler&
		get(void);
};

#endif /* CONNECTHANDLER_HPP_ */
