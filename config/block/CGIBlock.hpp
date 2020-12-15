/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIBlock.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/03 11:20:53 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIBLOCK_HPP_
# define CGIBLOCK_HPP_

#include <util/Optional.hpp>
#include <string>

class CGIBlock
{
	private:
		std::string m_name;
		Optional<std::string> m_path;

	public:
		CGIBlock();
		CGIBlock(std::string name);
		CGIBlock(const CGIBlock &other);

		virtual
		~CGIBlock();

		CGIBlock&
		operator=(const CGIBlock &other);

		CGIBlock&
		path(const std::string &path);

		inline const std::string&
		name(void) const
		{
			return (m_name);
		}

		inline const Optional<std::string>&
		path(void) const
		{
			return (m_path);
		}
};

#endif /* CGIBLOCK_HPP_ */
