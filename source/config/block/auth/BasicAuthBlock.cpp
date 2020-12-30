/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicAuthBlock.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 17:16:33 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/28 17:16:33 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/auth/BasicAuthBlock.hpp>
#include <encoding/default/base64/Base64.hpp>
#include <encoding/default/base64/Base64Decoder.hpp>
#include <stddef.h>
#include <util/Optional.hpp>
#include <string>

const std::string BasicAuthBlock::TYPE = "basic";
const std::string BasicAuthBlock::PRETTY_TYPE = "Basic";

BasicAuthBlock::BasicAuthBlock() :
		AuthBlock(),
		m_user(),
		m_password()
{
}

BasicAuthBlock::BasicAuthBlock(const std::string &user) :
		AuthBlock(),
		m_user(user),
		m_password()
{
}

BasicAuthBlock::BasicAuthBlock(const BasicAuthBlock &other) :
		AuthBlock(other),
		m_user(other.m_user),
		m_password(other.m_password)
{
}

BasicAuthBlock::~BasicAuthBlock()
{
}

BasicAuthBlock&
BasicAuthBlock::operator=(const BasicAuthBlock &other)
{
	AuthBlock::operator =(other);

	if (this != &other)
	{
		m_user = other.m_user;
		m_password = other.m_password;
	}

	return (*this);
}

BasicAuthBlock&
BasicAuthBlock::password(const std::string &password)
{
	m_password.set(password);

	return (*this);
}

bool
BasicAuthBlock::authorize(const std::string &credentials) const
{
	std::string decoded = Base64::decoder().decode(credentials);

	size_t pos = decoded.find(":");

	std::string user = decoded.substr(0, pos);
	std::string password = pos != std::string::npos ? decoded.erase(0, pos + 1) : "";

	if (user == m_user && ((password.empty() && m_password.absent()) || (m_password.present() && m_password.get() == password)))
		return (true);

	return (false);
}

const std::string&
BasicAuthBlock::type() const
{
	return (TYPE);
}

const std::string&
BasicAuthBlock::prettyType() const
{
	return (PRETTY_TYPE);
}
