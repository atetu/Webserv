/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inet6Address.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 20:47:35 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 20:47:35 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <net/address/Inet6Address.hpp>
#include <util/Convert.hpp>
#include <cstring>

Inet6Address::Inet6Address() :
		m_address()
{
	::memset(m_address, 0, sizeof(m_address));
}

Inet6Address::Inet6Address(byte address[16]) :
		m_address()
{
	::memcpy(m_address, address, sizeof(m_address));
}

Inet6Address::Inet6Address(const Inet6Address &other) :
		m_address()
{
	::memcpy(m_address, other.m_address, sizeof(m_address));
}

Inet6Address::~Inet6Address()
{
}

Inet6Address&
Inet6Address::operator =(const Inet6Address &other)
{
	if (this != &other)
		::memcpy(m_address, other.m_address, sizeof(m_address));

	return (*this);
}

std::vector<byte>
Inet6Address::address() const
{
	return (std::vector<byte>(m_address, m_address + sizeof(m_address)));
}

std::string
Inet6Address::hostAddress() const
{
	return (Convert::toString(m_address[0], 16)); // TODO Finish
}

InetAddress*
Inet6Address::clone() const
{
	return (new Inet6Address(*this));
}
