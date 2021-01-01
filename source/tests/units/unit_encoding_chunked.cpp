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

#include <encoding/default/chunk/ChunkDecoder.hpp>
#include <encoding/default/chunk/ChunkEncoder.hpp>
#include <tests/test_unit.hpp>
#include <util/log/LoggerFactory.hpp>
#include <string>

static std::string
encode(const char *str)
{
	return (ChunkEncoder(str).encode());
}

static std::string
decode(const char *str)
{
	return (ChunkDecoder(str).decode());
}

CASE("chunked", "encode")
{
	ASSERT(encode("Hello") == "5\r\nHello\r\n0\r\n\r\n");
	ASSERT(encode("World") == "5\r\nWorld\r\n0\r\n\r\n"); // TODO Shouldn't there be an empty CRLF between 2 chunks?

	return (0);
}

CASE("chunked", "decode")
{
	ASSERT(decode("5\r\nHello\r\n0\r\n\r\n") == "Hello");
	ASSERT(decode("5\r\nWorld\r\n0\r\n\r\n") == "World");

	return (0);
}

