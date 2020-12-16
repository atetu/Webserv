/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonValue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 18:50:40 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/11 18:50:40 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSONVALUE_HPP_
# define JSONVALUE_HPP_

#include <iostream>
#include <string>

class JsonValue
{
	public:
		enum Type
		{
			TYPE_ARRAY,
			TYPE_OBJECT,
			TYPE_STRING,
			TYPE_NUMBER,
			TYPE_DECIMAL,
			TYPE_BOOLEAN,
			TYPE_NULL,
			TYPE__SIZE
		};

	public:
		virtual
		~JsonValue()
		{
		}

		virtual JsonValue*
		clone() const = 0;

		virtual const Type
		type() const = 0;

		const std::string&
		typeString() const
		{
			return (typeStringStorage()[type()]);
		}

		static const std::string*
		typeStringStorage()
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

		virtual const std::string
		toJsonString() const = 0;

		template<typename T>
			inline bool
			instanceOf() const
			{
				return (!!cast<T>());
			}

		template<typename T>
			inline T*
			cast()
			{
				return (dynamic_cast<T*>(this));
			}

		template<typename T>
			inline const T*
			cast() const
			{
				return (dynamic_cast<const T*>(this));
			}
};

template<typename T>
	struct JsonTypeTraits
	{
			static const char *name;
			static const JsonValue::Type type;
			static const char *typeString;
	};

#define REGISTER_JSON_TYPE_TRAITS(cls, jsonType) \
    template<> \
		const char* JsonTypeTraits<cls>::name = #cls; \
	template<> \
		const JsonValue::Type JsonTypeTraits<cls>::type = jsonType; \
	template<> \
		const char* JsonTypeTraits<cls>::typeString = JsonValue::typeStringStorage()[jsonType].c_str();

std::ostream&
operator <<(std::ostream &stream, const JsonValue &jsonValue);

#endif
