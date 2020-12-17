/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64Encoder.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:28:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:28:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64ENCODER_HPP_
# define BASE64ENCODER_HPP_

#include <encoding/Encoder.hpp>
#include <string>

class Base64Encoder :
		public Encoder
{
	public:
		Base64Encoder();
		Base64Encoder(const Base64Encoder &other);

		virtual
		~Base64Encoder();

		Base64Encoder&
		operator=(const Base64Encoder &other);

		virtual std::string
		encode(const std::string &input);
};

#endif /* BASE64ENCODER_HPP_ */
