/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_encoding_chunked.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 00:58:35 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/26 00:58:35 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/body/encoding/chunked/ChunkDecoder.hpp>
#include <http/body/encoding/chunked/ChunkEncoder.hpp>
#include <libs/ft.hpp>
#include <stddef.h>
#include <tests/test_unit.hpp>
#include <string>

class ChunkEncoder;

static std::string
encode(const char *str)
{
	return (ChunkEncoder().encode(str));
}

static std::string
decode(const char *str)
{
	return (ChunkDecoder(false).decode(str));
}

static std::string
decode2(const char *str, const char *str2)
{
	ChunkDecoder decoder(false);

	size_t consumed = 0;
	std::string out;
	decoder.consume(str, out, consumed);
	decoder.consume(str2, out, consumed);

	return (out);
}

CASE("chunked", "encode")
{
	ASSERT(encode("Hello") == "5\r\nHello\r\n0\r\n\r\n");
	ASSERT(encode("World") == "5\r\nWorld\r\n0\r\n\r\n");

	return (0);
}

CASE("chunked", "decode")
{
	ASSERT(decode("5\r\nHello\r\n0\r\n\r\n") == "Hello");
	ASSERT(decode("5\r\nWorld\r\n0\r\n\r\n") == "World");

	return (0);
}

CASE("chunked", "decode2")
{
	ASSERT(decode2("5\r\nHello\r\n", "0\r\n\r\n") == "Hello");
	ASSERT(decode2("5\r\nWorld\r\n", "0\r\n\r\n") == "World");

	ASSERT(decode2("5\r\nHello\r\n0\r\n", "\r\n") == "Hello");
	ASSERT(decode2("5\r\nWorld", "\r\n0\r\n\r\n") == "World");

	return (0);
}

