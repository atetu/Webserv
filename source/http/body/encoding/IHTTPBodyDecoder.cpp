/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IHTTPBodyDecoder.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 01:00:24 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/09 01:00:24 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/body/encoding/IHTTPBodyDecoder.hpp>

IHTTPBodyDecoder::~IHTTPBodyDecoder()
{
}

std::string
IHTTPBodyDecoder::decode(const std::string &input)
{
	std::string out;

	consume(input, out);

	return (out);
}

void
IHTTPBodyDecoder::cleanup()
{
}
