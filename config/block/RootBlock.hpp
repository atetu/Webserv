/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootBlock.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 00:13:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/26 00:13:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOTBLOCK_HPP_
# define ROOTBLOCK_HPP_

#include <util/Optional.hpp>
#include <string>

class RootBlock
{
	public:
		RootBlock();
		virtual
		~RootBlock();

		inline Optional<std::string>&
		root(void) const
		{
			static Optional<std::string> empty;

			return (empty);
		}
};

#endif /* ROOTBLOCK_HPP_ */
