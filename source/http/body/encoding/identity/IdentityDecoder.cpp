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
}

IdentityDecoder::~IdentityDecoder()
{
}

size_t
IdentityDecoder::consume(const std::string &in, std::string &out)
{
	size_t willConsume = std::min(in.size(), size_t(m_contentLength - m_consumed));

	out.append(in.c_str(), in.c_str() + willConsume);

	m_consumed += willConsume;

	return (willConsume);
}

void
IdentityDecoder::cleanup()
{
	if (m_isAllocated)
		delete this;
}
