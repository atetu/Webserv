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

		virtual const std::string&
		tag() const = 0;
};

#endif /* LOGGER_HPP_ */
