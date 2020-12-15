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
		Optional<std::vector<Mime*> > m_defines;

	public:
		MimeBlock();
		MimeBlock(const MimeBlock &other);

		virtual
		~MimeBlock();

		MimeBlock&
		operator =(const MimeBlock &other);

		MimeBlock&
		includes(const std::vector<std::string> &includes);

		MimeBlock&
		defines(const std::vector<Mime*> &defines);

		const Optional<std::vector<std::string> >&
		includes()
		{
			return (m_includes);
		}

		const Optional<std::vector<Mime*> >&
		defines()
		{
			return (m_defines);
		}
};

#endif /* MIMEBLOCK_HPP_ */
