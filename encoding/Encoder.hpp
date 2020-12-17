/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Encoder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:09:55 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:09:55 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENCODER_HPP_
# define ENCODER_HPP_

#include <string>

class Encoder
{
	public:
		virtual
		~Encoder(void)
		{
		}

		virtual std::string
		encode(const std::string &input) = 0;
};

#endif /* ENCODER_HPP_ */
