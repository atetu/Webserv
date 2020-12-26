/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_json.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 00:05:08 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/26 00:05:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <tests/test_unit.hpp>
#include <util/json/JsonReader.hpp>
#include <util/json/JsonValue.hpp>
#include <util/log/LoggerFactory.hpp>

static Logger &LOG = LoggerFactory::get("util/json/*");

static JsonValue *g_temporary_storage = NULL;

static JsonValue&
autoDelete(JsonValue *value)
{
	if (g_temporary_storage)
		delete g_temporary_storage;

	return (*(g_temporary_storage = value));
}

static JsonValue&
json(const char *str)
{
	return (autoDelete(JsonReader(str).read()));
}

CASE("json", "parsing number")
{
	ASSERT(json("0").instanceOf<JsonNumber>());
	ASSERT(json("42").instanceOf<JsonNumber>());
	ASSERT(json("-42").instanceOf<JsonNumber>());

	ASSERT(json("0").cast<JsonNumber>()->operator int() == 0);
	ASSERT(json("42").cast<JsonNumber>()->operator int() == 42);
	ASSERT(json("-42").cast<JsonNumber>()->operator int() == -42);

	return (0);
}

CASE("json", "parsing decimal")
{
	ASSERT(json("0.0").instanceOf<JsonDecimal>());
	ASSERT(json("4.2").instanceOf<JsonDecimal>());
	ASSERT(json("-4.2").instanceOf<JsonDecimal>());

	ASSERT(json("0.0").cast<JsonDecimal>()->operator double() == 0.0);
	ASSERT(json("4.2").cast<JsonDecimal>()->operator double() == 4.2);
	ASSERT(json("-4.2").cast<JsonDecimal>()->operator double() == -4.2);

	return (0);
}

CASE("json", "parsing true")
{
	ASSERT(json("true").instanceOf<JsonBoolean>());
	ASSERT(json("true").cast<JsonBoolean>()->operator bool() == true);

	return (0);
}

CASE("json", "parsing false")
{
	ASSERT(json("false").instanceOf<JsonBoolean>());
	ASSERT(json("false").cast<JsonBoolean>()->operator bool() == false);

	return (0);
}

CASE("json", "parsing null")
{
	ASSERT(json("null").instanceOf<JsonNull>());

	return (0);
}

CASE("json", "parsing string")
{
	ASSERT(json("\"Hello\"").instanceOf<JsonString>());

	ASSERT(json("\"Hello\"").cast<JsonString>()->operator std::string() == "Hello");

	return (0);
}

//CASE("json", "parsing string with escape")
//{
//	ASSERT(json("\"Hello\\\"\"").instanceOf<JsonString>());
//
//	ASSERT(json("\"Hello\\\"\"").cast<JsonString>()->operator std::string() == "Hello\\\"");
//
//	return (0);
//}

CASE("json", "parsing empty object")
{
	ASSERT(json("{}").instanceOf<JsonObject>());
	ASSERT(json("{}").cast<JsonObject>()->empty());

	return (0);
}

CASE("json", "parsing empty array")
{
	ASSERT(json("[]").instanceOf<JsonArray>());
	ASSERT(json("[]").cast<JsonArray>()->empty());

	return (0);
}

