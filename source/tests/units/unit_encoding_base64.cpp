/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_encoding_base64.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 00:54:37 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/26 00:54:37 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <encoding/default/base64/Base64.hpp>
#include <encoding/default/base64/Base64Encoder.hpp>
#include <tests/test_unit.hpp>
#include <util/log/LoggerFactory.hpp>

static std::string
encode(const char *str)
{
	return (Base64::encoder().encode(str));
}

static std::string
decode(const char *str)
{
	return (Base64::decoder().decode(str));
}

CASE("base64", "encode")
{
	ASSERT(encode("Hello") == "SGVsbG8=");
	ASSERT(encode("World") == "V29ybGQ=");

	return (0);
}

CASE("base64", "decode")
{
	ASSERT(decode("SGVsbG8=") == "Hello");
	ASSERT(decode("V29ybGQ=") == "World");

	return (0);
}
