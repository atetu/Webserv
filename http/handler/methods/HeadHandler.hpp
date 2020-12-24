/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:51:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:51:44 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADHANDLER_HPP_
# define HEADHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>
#include <http/HTTPRequest.hpp>

class HeadHandler :
		public HTTPMethodHandler
{
	public:
		HeadHandler();

		virtual
		~HeadHandler();

		GenericHTTPResponse*
		handle(HTTPRequest &request);

	public:
		static HeadHandler&
		get(void);
};

#endif /* HeadHANDLER_HPP_ */
