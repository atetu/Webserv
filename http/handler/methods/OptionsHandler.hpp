/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionsHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 10:42:43 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/25 10:42:43 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONSHANDLER_HPP_
# define OPTIONSHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>

class OptionsHandler :
		public HTTPMethodHandler
{
	public:
		OptionsHandler();

		virtual
		~OptionsHandler();

		HTTPResponse*
		handle(HTTPRequest &request);

	public:
		static OptionsHandler&
		get(void);
};

#endif /* OptionsHANDLER_HPP_ */
