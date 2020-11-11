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

		virtual const std::string
		toJsonString() const = 0;

		template<typename T>
			bool
			instanceOf() const
			{
				return (!!dynamic_cast<const T*>(this));
			}
};

std::ostream&
operator <<(std::ostream &stream, const JsonValue &jsonValue);

#endif
