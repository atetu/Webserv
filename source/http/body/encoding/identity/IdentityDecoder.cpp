/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IdentityDecoder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 01:26:21 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/09 01:26:21 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/body/encoding/identity/IdentityDecoder.hpp>
#include <iostream>
#include <string>

IdentityDecoder::IdentityDecoder(bool isAllocated, long long contentLength) :
		m_isAllocated(isAllocated),
		m_contentLength(contentLength),
		m_consumed()
{
//	std::cout << "IdentityDecoder" << contentLength << std::endl;
}

IdentityDecoder::~IdentityDecoder()
{
}

bool
IdentityDecoder::consume(std::string &out, char c)
{
	out += c;

	return (++m_consumed >= m_contentLength);
}

void
IdentityDecoder::cleanup()
{
	if (m_isAllocated)
		delete this;
}
