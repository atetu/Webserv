/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 16:07:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 16:07:47 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP_
# define LOGGER_HPP_

#include <iostream>
#include <string>
#include <util/log/LogLevel.hpp>

class Logger
{
	public:
		virtual
		~Logger()
		{
		}

		virtual std::ostream&
		trace() const = 0;

		virtual std::ostream&
		debug() const = 0;

		virtual std::ostream&
		info() const = 0;

		virtual std::ostream&
		warn() const = 0;

		virtual std::ostream&
		error() const = 0;

		virtual std::ostream&
		fatal() const = 0;

		virtual std::ostream&
		log(LogLevel &level) const = 0;

		virtual bool
		isEnabled(LogLevel &logLevel) const = 0;

		virtual const std::string&
		tag() const = 0;

		virtual inline bool
		isTraceEnabled() const
		{
			return (isEnabled(*LogLevel::TRACE));
		}

		virtual inline bool
		isDebugEnabled() const
		{
			return (isEnabled(*LogLevel::DEBUG));
		}

		virtual inline bool
		isInfoEnabled() const
		{
			return (isEnabled(*LogLevel::INFO));
		}

		virtual inline bool
		isWarnEnabled() const
		{
			return (isEnabled(*LogLevel::WARN));
		}

		virtual inline bool
		isErrorEnabled() const
		{
			return (isEnabled(*LogLevel::ERROR));
		}

		virtual inline bool
		isFatalEnabled() const
		{
			return (isEnabled(*LogLevel::FATAL));
		}
};

#endif /* LOGGER_HPP_ */
