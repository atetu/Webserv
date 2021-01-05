/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CharacterUtils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 01:16:40 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/05 01:16:40 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libs/ft.hpp>
#include <util/CharacterUtils.hpp>

CharacterUtils::CharacterUtils()
{
}

CharacterUtils::~CharacterUtils()
{
}

CharacterUtils::CharacterUtils(const CharacterUtils &other)
{
	(void)other;
}

CharacterUtils&
CharacterUtils::operator=(const CharacterUtils &other)
{
	(void)other;
	return (*this);
}

bool
CharacterUtils::InsensitiveCompare::operator ()(const unsigned char &c1, const unsigned char &c2) const
{
	return (ft::tolower(c1) < ft::tolower(c2));
}
