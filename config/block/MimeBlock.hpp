/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeBlock.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:23:13 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:23:13 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMEBLOCK_HPP_
# define MIMEBLOCK_HPP_

#include <http/mime/Mime.hpp>
#include <util/Optional.hpp>
#include <string>
#include <vector>

class MimeBlock
{
	private:
		Optional<std::vector<std::string> > m_includes;
		Optional<std::vector<Mime> > m_define;

	public:
		MimeBlock();

		virtual
		~MimeBlock();
};

#endif /* MIMEBLOCK_HPP_ */
