/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:43:31 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:43:31 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUTHANDLER_HPP_
# define PUTHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>
#include <io/File.hpp>

class PutHandler :
		public HTTPMethodHandler
{
	public:
		PutHandler();

		virtual
		~PutHandler();

		HTTPResponse*
		handle(HTTPRequest &request);

		int
		checkExtension(HTTPRequest &request, File &file);

	public:
		static PutHandler&
		get(void);
};

#endif /* PutHANDLER_HPP_ */
