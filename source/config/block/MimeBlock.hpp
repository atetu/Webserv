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
#include <list>
#include <string>

class MimeBlock
{
	private:
		Optional<std::list<std::string> > m_includes;
		Optional<std::list<Mime const*> > m_defines;

	private:
		MimeBlock(const MimeBlock &other);

		MimeBlock&
		operator =(const MimeBlock &other);

	public:
		MimeBlock();

		virtual
		~MimeBlock();

		MimeBlock&
		includes(const std::list<std::string> &includes);

		MimeBlock&
		defines(const std::list<Mime const*> &defines);

		const Optional<std::list<std::string> >&
		includes() const
		{
			return (m_includes);
		}

		const Optional<std::list<Mime const*> >&
		defines() const
		{
			return (m_defines);
		}
};

#endif /* MIMEBLOCK_HPP_ */
