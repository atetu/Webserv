/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:26:37 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:26:37 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_HPP_
# define BASE64_HPP_

#include <encoding/default/base64/Base64Decoder.hpp>
#include <encoding/default/base64/Base64Encoder.hpp>
#include <encoding/EncodingRegistry.hpp>
#include <cctype>
#include <string>

/**
 * @author https://stackoverflow.com/a/180949/7292958
 */
class Base64
{
	public:
		static const std::string NAME;
		static const std::string CHARACTERS;

		static Base64Encoder ENCODER;
		static Base64Decoder DECODER;

	private:
		Base64();
		Base64(const Base64 &other);

		Base64&
		operator=(const Base64 &other);

	public:
		virtual
		~Base64();

		static void
		load();

		static inline Base64Encoder&
		encoder()
		{
			return (ENCODER);
		}

		static inline Base64Decoder&
		decoder()
		{
			return (DECODER);
		}

		static inline bool
		isValidCharacter(char c)
		{
			return (std::isalnum(c) || (c == '+') || (c == '/'));
		}
};

#endif /* BASE64_HPP_ */
