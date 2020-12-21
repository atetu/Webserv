/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatusLine.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 19:53:31 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/20 19:53:31 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSLINE_HPP_
# define HTTPSTATUSLINE_HPP_

#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <string>

class HTTPStatusLine
{
	private:
		HTTPVersion m_version;
		HTTPStatus m_status;

	public:
		HTTPStatusLine(void);
		HTTPStatusLine(const HTTPStatus &status);
		HTTPStatusLine(const HTTPVersion &version, const HTTPStatus &status);
		HTTPStatusLine(const HTTPStatusLine &other);

		virtual
		~HTTPStatusLine();

		HTTPStatusLine&
		operator=(const HTTPStatusLine &other);

		std::string
		format(void) const;
};

#endif /* HTTPSTATUSLINE_HPP_ */
