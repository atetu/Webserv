/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPBodyDecoder.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 01:00:24 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/09 01:00:24 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IHTTPBODYDECODER_HPP_
# define IHTTPBODYDECODER_HPP_

#include <string>

class IHTTPBodyDecoder
{
	public:
		virtual
		~IHTTPBodyDecoder();

		/**
		 * Consume a character.
		 *
		 * @param out Output storage.
		 * @param c Character to consume.
		 * @return Whether or not the decoding is complete.
		 */
		virtual bool
		consume(std::string &out, char c) = 0;

		/**
		 * Cleanup the memory after usage.
		 *
		 * For stateful and allocated, that mean delete-ing itself.
		 * For stateless, nothing is done.
		 */
		virtual void
		cleanup();
};

#endif /* IHTTPBODYDECODER_HPP_ */
