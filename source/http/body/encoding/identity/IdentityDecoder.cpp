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
IdentityDecoder::consume(const std::string &in, std::string &out, size_t &consumed)
{
	consumed = std::min(in.size(), size_t(m_contentLength - m_consumed));

	out.append(in.c_str(), in.c_str() + consumed);

	m_consumed += consumed;
	if (m_consumed > m_maxBodySize)
		throw Exception("Too large payload");

	return (m_consumed == m_contentLength);
}

void
IdentityDecoder::cleanup()
{
	if (m_isAllocated)
		delete this;
}
