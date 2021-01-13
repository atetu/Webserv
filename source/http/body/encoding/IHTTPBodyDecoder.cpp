/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPBodyDecoder.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 01:00:24 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/13 16:26:16 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/body/encoding/IHTTPBodyDecoder.hpp>
#include <iostream>

IHTTPBodyDecoder::~IHTTPBodyDecoder()
{
}

std::string
IHTTPBodyDecoder::decode(const std::string &input)
{
	std::string out;

	std::string copy = input;
	size_t consumed = 0;
	std::cout << "in\n";
	bool finished = false;
	while(!finished)
	{
		consumed = 0;
		finished = consume(copy, out, consumed);
			std::cout << "out : " << out << std::endl;

		copy.erase(0, consumed);
	}

	return (out);
}

void
IHTTPBodyDecoder::cleanup()
{
}
