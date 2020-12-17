/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Decoder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:09:55 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:09:55 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECODER_HPP_
# define DECODER_HPP_

#include <string>

class Decoder
{
	public:
		virtual
		~Decoder(void)
		{
		}

		virtual std::string
		decode(const std::string &input) = 0;
};

#endif /* DECODER_HPP_ */
