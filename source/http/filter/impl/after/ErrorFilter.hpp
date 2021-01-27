/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorFilter.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 00:30:02 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/27 13:35:46 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORFILTER_HPP_
# define ERRORFILTER_HPP_

#include <http/filter/Filter.hpp>
#include <util/Singleton.hpp>
#include <exception/IOException.hpp>

class Logger;

class ErrorFilter :
		public Filter,
		public Singleton<ErrorFilter>
{
	public:
		static Logger &LOG;

	public:
		ErrorFilter();
		ErrorFilter(const ErrorFilter &other);

		virtual
		~ErrorFilter();

		ErrorFilter&
		operator=(const ErrorFilter &other);

		void
		doFilter(HTTPClient &client, HTTPRequest &request, HTTPResponse &response, FilterChain &next);

		// private:
		// 	template<typename T>
		// 		inline static IOException
		// 		resetErrnoAndReturn(const T &t)
		// 		{
		// 			errno = 0;
		// 			return (t);
		// 		}

		// 	inline IOException
		// 	ioException() const
		// 	{
		// 		return (resetErrnoAndReturn(IOException(, errno)));
		// 	}
};

#endif /* ERRORFILTER_HPP_ */
