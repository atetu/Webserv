/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inet4Address.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 20:46:52 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 20:46:52 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <net/address/Inet4Address.hpp>
#include <util/Convert.hpp>
#include <cstring>
#include <iostream>

Inet4Address::Inet4Address() :
		m_address()
{
	::memset(m_address, 0, sizeof(m_address));
}

Inet4Address::Inet4Address(byte address[4]) :
		m_address()
{
	::memcpy(m_address, address, sizeof(m_address));
}

Inet4Address::Inet4Address(const Inet4Address &other) :
		m_address()
{
	::memcpy(m_address, other.m_address, sizeof(m_address));
}

Inet4Address::~Inet4Address()
{
}

Inet4Address&
Inet4Address::operator =(const Inet4Address &other)
{
	if (this != &other)
		::memcpy(m_address, other.m_address, sizeof(m_address));

	return (*this);
}

std::vector<byte>
Inet4Address::address() const
{
	return (std::vector<byte>(m_address, m_address + sizeof(m_address)));
}

std::string
Inet4Address::hostAddress() const
{
	return (Convert::toString((int)m_address[0]) + "." + Convert::toString((int)m_address[1]) + "." + Convert::toString((int)m_address[2]) + "." + Convert::toString((int)m_address[3]));
}

InetAddress*
Inet4Address::clone() const
{
	return (new Inet4Address(*this));
}

Inet4Address
Inet4Address::parse(unsigned int x)
{
	return (Inet4Address((byte*)&x));
}
