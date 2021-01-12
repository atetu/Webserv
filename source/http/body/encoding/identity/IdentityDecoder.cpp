/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IdentityDecoder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 01:26:21 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/12 17:47:08 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/body/encoding/identity/IdentityDecoder.hpp>
#include <iostream>
#include <string>

IdentityDecoder::IdentityDecoder(bool isAllocated, long long contentLength, long long maxBodySize) :
		m_isAllocated(isAllocated),
		m_contentLength(contentLength),
		m_consumed(),
		m_maxBodySize(maxBodySize)
{
}

IdentityDecoder::~IdentityDecoder()
{
}

bool
IdentityDecoder::consume(std::string &out, char c)
{
	out += c;
	m_consumed++;
//	std::cout << "consumed: " <<  m_consumed << std::endl;
//	std::cout << "max: " <<  m_maxBodySize << std::endl;
	if (m_consumed > m_maxBodySize)
	{
//		std::cout << "ici\n";
		throw Exception("Too large payload");
	//	return (true);
	}
	return (++m_consumed >= m_contentLength);
}

void
IdentityDecoder::cleanup()
{
	if (m_isAllocated)
		delete this;
}
