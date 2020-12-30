/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogLevel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 16:09:19 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 16:09:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGLEVEL_HPP_
# define LOGLEVEL_HPP_

#include <util/Enum.hpp>

class LogLevel :
		public Enum<LogLevel>
{
	public:
		static LogLevel *TRACE;
		static LogLevel *DEBUG;
		static LogLevel *INFO;
		static LogLevel *WARN;
		static LogLevel *ERROR;
		static LogLevel *FATAL;

		static const LogLevel *ACTIVE;
};

#endif /* LOGLEVEL_HPP_ */
