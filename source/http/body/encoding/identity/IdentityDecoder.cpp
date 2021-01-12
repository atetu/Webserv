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

size_t
IdentityDecoder::consume(const std::string &in, std::string &out)
{
	size_t willConsume = std::min(in.size(), size_t(m_contentLength - m_consumed));
	std::cout << "willConsume: " << willConsume << std::endl;
	std::cout << "in.size(): " << in.size() << std::endl;
	std::cout << "size_t(m_contentLength - m_consumed): " << size_t(m_contentLength - m_consumed) << std::endl;

	out.append(in.c_str(), in.c_str() + willConsume);

	m_consumed += willConsume;
	if (m_consumed > m_maxBodySize)
		throw Exception("Too large payload");

	return (willConsume);
}

void
IdentityDecoder::cleanup()
{
	if (m_isAllocated)
		delete this;
}
