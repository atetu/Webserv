/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/01 10:40:19 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP_
# define CGI_HPP_

#include <util/Optional.hpp>
#include <string>

class CGI
{
	public: // pourquoi public?
		std::string m_name;
		Optional<std::string> m_path;

	public:
		CGI();
		CGI(std::string name);
		CGI(const CGI &other);

		virtual
		~CGI();

		CGI&
		operator=(const CGI &other);

		CGI&
		path(const std::string & path);
};

#endif /* SRC_CONFIG_BLOCK_CGI_HPP_ */
