/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/03 11:20:53 by alicetetu        ###   ########.fr       */
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

		const std::string
		name(void) const;

		const Optional<std::string>
		path(void) const;
};

#endif /* SRC_CONFIG_BLOCK_CGI_HPP_ */
