/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonArray.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 21:55:45 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/11 21:55:45 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSONARRAY_HPP_
# define JSONARRAY_HPP_

#include <util/json/JsonStructure.hpp>
#include <util/json/JsonValue.hpp>
#include <iterator>
#include <string>
#include <vector>

class JsonArray :
		public JsonStructure
{
	public:
		typedef std::vector<JsonValue*> Container;
		typedef typename Container::size_type size_type;
		typedef typename Container::iterator iterator;
		typedef typename Container::const_iterator const_iterator;

	private:
		Container m_value;

	public:
		JsonArray(void) :
				m_value()
		{
		}

		JsonArray(std::vector<JsonValue*> value) :
				m_value(value)
		{
		}

		JsonArray(const JsonArray &other) :
				m_value()
		{
			operator =(other);
		}

		virtual
		~JsonArray(void)
		{
			clear();
		}

		JsonArray&
		operator =(const JsonArray &other)
		{
			if (this != &other)
			{
				const_iterator it = other.m_value.begin();
				const_iterator ite = other.m_value.end();

				clear();

				for (; it != ite; it++)
					m_value.insert(m_value.end(), (*it)->clone());
			}

			return (*this);
		}

		JsonValue&
		operator [](int index)
		{
			return (*(m_value[index]));
		}

		const JsonValue&
		operator [](int index) const
		{
			return (*(m_value[index]));
		}

		operator Container(void)
		{
			return (m_value);
		}

		operator Container(void) const
		{
			return (m_value);
		}

		void
		add(JsonValue *value)
		{
			m_value.push_back(value);
		}

		inline iterator
		begin()
		{
			return (m_value.begin());
		}

		inline iterator
		end()
		{
			return (m_value.end());
		}

		inline const_iterator
		begin() const
		{
			return (m_value.begin());
		}

		inline const_iterator
		end() const
		{
			return (m_value.end());
		}

		virtual void
		clear(void)
		{
			if (m_value.empty())
				return;

			iterator it = m_value.begin();
			iterator ite = m_value.end();

			for (; it != ite; it++) // TODO Use simple for-loop...
				delete *it;

			return (m_value.clear());
		}

		inline bool
		empty() const
		{
			return (m_value.empty());
		}

		inline Container::size_type
		size() const
		{
			return (m_value.size());
		}

		JsonValue*
		clone(void) const
		{
			return (new JsonArray(*this));
		}

		const Type
		type(void) const
		{
			return (TYPE_ARRAY);
		}

		const std::string
		toJsonString(void) const;

		bool
		equals(const JsonValue &other) const
		{
			const JsonArray *casted = dynamic_cast<JsonArray const*>(&other);
			if (casted)
			{
				size_type size = this->size();
				if (size != casted->size())
					return (false);

				for (size_type index = 0; index < size; index++)
				{
					if (this->operator [](index) != casted->operator [](index))
						return (false);
				}

				return (true);
			}

			return (false);
		}
};

#endif /* JSONARRAY_HPP_ */
