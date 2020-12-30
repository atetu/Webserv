/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuthBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 17:16:20 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/28 17:16:20 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/AuthBlock.hpp>

AuthBlock::AuthBlock()
{
}

AuthBlock::AuthBlock(const AuthBlock &other)
{
	(void)other;
}

AuthBlock::~AuthBlock()
{
}

AuthBlock&
AuthBlock::operator=(const AuthBlock &other)
{
	(void)other;

	return (*this);
}

AuthBlock&
AuthBlock::realm(const std::string &realm)
{
	m_realm.set(realm);

	return (*this);
}
