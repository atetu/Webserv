/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TraceHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:42:43 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:42:43 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRACEHANDLER_HPP_
# define TRACEHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>

class TraceHandler :
		public HTTPMethodHandler
{
	public:
		TraceHandler();

		virtual
		~TraceHandler();

		HTTPResponse*
		handle(HTTPRequest &request);

	public:
		static TraceHandler&
		get(void);
};

#endif /* TraceHANDLER_HPP_ */
