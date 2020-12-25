/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:32:53 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/10 16:05:57 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHOD_HPP_
# define HTTPMETHOD_HPP_

#include <util/Enum.hpp>

class HTTPMethodHandler;

class HTTPMethod :
		public Enum<HTTPMethod>
{
	private:
		HTTPMethodHandler *m_handler;

	public:
		HTTPMethod(void);
		HTTPMethod(HTTPMethodHandler *handler);
		HTTPMethod(const HTTPMethod &other);

		virtual
		~HTTPMethod(void);

		HTTPMethod&
		operator =(const HTTPMethod &other);

		HTTPMethodHandler&
		handler(void) const;

	public:
		static HTTPMethod *GET;
		static HTTPMethod *HEAD;
		static HTTPMethod *POST;
		static HTTPMethod *PUT;
		static HTTPMethod *DELETE;
		static HTTPMethod *CONNECT;
		static HTTPMethod *OPTIONS;
		static HTTPMethod *TRACE;
};

#endif /* HTTPMETHOD_HPP_ */
