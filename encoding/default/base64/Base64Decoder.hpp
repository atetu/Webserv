/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64Decoder.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:28:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:28:15 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64DECODER_HPP_
# define BASE64DECODER_HPP_

#include <encoding/Decoder.hpp>
#include <string>

class Base64Decoder :
		public Decoder
{
	public:
		Base64Decoder();
		Base64Decoder(const Base64Decoder &other);

		virtual
		~Base64Decoder();

		Base64Decoder&
		operator=(const Base64Decoder &other);

		virtual std::string
		decode(const std::string &input);
};

#endif /* BASE64DECODER_HPP_ */
