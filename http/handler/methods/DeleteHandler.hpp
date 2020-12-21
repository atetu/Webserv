/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:13:28 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:13:28 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETEHANDLER_HPP_
# define DELETEHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>
#include <http/HTTPRequest.hpp>

class DeleteHandler :
		public HTTPMethodHandler
{
	public:
		DeleteHandler();

		virtual
		~DeleteHandler();

		GenericHTTPResponse*
		handle(HTTPRequest &request);

	public:
		static DeleteHandler&
		get(void);
};

#endif /* DELETEHANDLER_HPP_ */
