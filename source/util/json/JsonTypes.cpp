/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 02:16:34 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/16 02:16:34 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/json/JsonValue.hpp>
#include <util/json/JsonArray.hpp>
#include <util/json/JsonBoolean.hpp>
#include <util/json/JsonDecimal.hpp>
#include <util/json/JsonNull.hpp>
#include <util/json/JsonNumber.hpp>
#include <util/json/JsonObject.hpp>
#include <util/json/JsonReader.hpp>
#include <util/json/JsonString.hpp>

REGISTER_JSON_TYPE_TRAITS(JsonArray, JsonValue::TYPE_ARRAY);
REGISTER_JSON_TYPE_TRAITS(JsonObject, JsonValue::TYPE_OBJECT);
REGISTER_JSON_TYPE_TRAITS(JsonString, JsonValue::TYPE_STRING);
REGISTER_JSON_TYPE_TRAITS(JsonNumber, JsonValue::TYPE_NUMBER);
REGISTER_JSON_TYPE_TRAITS(JsonDecimal, JsonValue::TYPE_DECIMAL);
REGISTER_JSON_TYPE_TRAITS(JsonBoolean, JsonValue::TYPE_BOOLEAN);
REGISTER_JSON_TYPE_TRAITS(JsonNull, JsonValue::TYPE_NULL);
