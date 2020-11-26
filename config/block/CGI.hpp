/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 11:34:15 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP_
# define CGI_HPP_

#include <util/Optional.hpp>
#include <string>

class CGI
{
	public:
		std::string m_name;
		Optional<std::string> m_path;

	public:
		CGI();
		CGI(const CGI &other);

		virtual
		~CGI();

		CGI&
		operator=(const CGI &other);
};

#endif /* SRC_CONFIG_BLOCK_CGI_HPP_ */
