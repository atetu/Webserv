/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:51:44 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 18:51:44 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETHANDLER_HPP_
# define GETHANDLER_HPP_

#include <http/handler/HTTPMethodHandler.hpp>
#include <io/File.hpp>
#include <string>

class GetHandler :
		public HTTPMethodHandler
{
	public:
		GetHandler();

		virtual
		~GetHandler();

		HTTPResponse*
		handle(HTTPRequest &request);

	private:
		std::string
		listing(const URL &url, const File &file);

	public:
		static GetHandler&
		get(void);
};

#endif /* GETHANDLER_HPP_ */
