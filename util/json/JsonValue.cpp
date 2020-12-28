/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonValue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 18:50:40 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/11 18:50:40 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JsonValue.hpp"

const std::string&
JsonValue::typeString() const
{
	return (typeStringStorage()[type()]);
}

const std::string*
JsonValue::typeStringStorage()
{
	static std::string strings[TYPE__SIZE] = {
		[TYPE_ARRAY] = "ARRAY",
		[TYPE_OBJECT] = "OBJECT",
		[TYPE_STRING] = "STRING",
		[TYPE_NUMBER] = "NUMBER",
		[TYPE_DECIMAL] = "DECIMAL",
		[TYPE_BOOLEAN] = "BOOLEAN",
		[TYPE_NULL] = "NULL" };

	return ((std::string*)strings);
}

std::ostream&
operator <<(std::ostream &stream, const JsonValue &jsonValue)
{
	return (stream << jsonValue.toJsonString());
}
