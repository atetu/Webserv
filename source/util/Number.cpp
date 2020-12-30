/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Number.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 22:43:54 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/30 22:43:54 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/Number.hpp>

const std::string Number::BIN = "01";
const std::string Number::DECIMAL = "0123456789";
const std::string Number::HEX = "0123456789ABCDEF";

Number::Number()
{
}

Number::Number(const Number &other)
{
	(void)other;
}

Number::~Number()
{
}

Number&
Number::operator=(const Number &other)
{
	(void)other;

	return (*this);
}

