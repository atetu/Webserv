/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestPathParser.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 21:10:12 by ecaceres          #+#    #+#             */
/*   Updated: 2020/01/01 21:10:12 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/Exception.hpp>
#include <http/parser/HTTPRequestPathParser.hpp>
#include <util/Number.hpp>

HTTPRequestPathParser::HTTPRequestPathParser() :
		m_state(S_PATH),
		m_path("/"),
		m_queryKey(),
		m_queryValue(),
		m_hexBeforeState(),
		m_hexStoreTarget(),
		m_hex(),
		m_query(),
		m_fragment()
{
	m_path.reserve(60);
}

void
HTTPRequestPathParser::consume(char c)
{
	switch (m_state)
	{
		case S_PATH:
		{
			if (c == ' ')
				m_state = S_END;
			else if (c == '%')
				hexStart(&m_path);
			else if (c == '?')
			{
				m_query.set();
				m_state = S_QUERY_STRING_KEY;
			}
			else if (c == '#')
			{
				m_fragment.set();
				commitQueryKeyValue(S_FRAGMENT);
			}
			else
				m_path += c;

			break;
		}

		case S_QUERY_STRING_KEY:
		{
			if (c == '=')
				m_state = S_QUERY_STRING_VALUE;
			else if (c == '%')
				hexStart(&m_queryKey);
			else if (c == '#')
			{
				m_fragment.set();
				commitQueryKeyValue(S_FRAGMENT);
			}
//			else if (c == '+')
//				m_queryKey += ' '; // TODO @atetu can you explain???
			else if (c == ' ')
				commitQueryKeyValue(S_END);
			else
				m_queryKey += c;

			break;
		}

		case S_QUERY_STRING_VALUE:
		{
			if (c == '%')
				hexStart(&m_queryValue);
			else if (c == '&')
				commitQueryKeyValue(S_QUERY_STRING_KEY);
			else if (c == '#')
			{
				m_fragment.set();
				commitQueryKeyValue(S_FRAGMENT);
			}
			else if (c == ' ')
				commitQueryKeyValue(S_END);
			else
				m_queryValue += c;

			break;
		}

		case S_FRAGMENT:
		{
			if (c == ' ')
				m_state = S_END;
			else if (c == '%')
				hexStart(&m_fragment.get());
			else
				m_fragment.get() += c;

			break;
		}

		case S_HEX_START:
		{
			m_hex.clear();
			m_hex += c;

			m_state = S_HEX_END;

			break;
		}

		case S_HEX_END:
		{
			m_hex += c;

			char h = Number::parse<char>(m_hex, Number::HEX);
			if (h == 0)
				throw Exception("Decoded hex value cannot be a null terminator");

			*m_hexStoreTarget += h;
			m_state = m_hexBeforeState;

			break;
		}

		case S_END:
			break;

	}
}

void
HTTPRequestPathParser::commitQueryKeyValue(State nextState)
{
	m_query.get()[m_queryKey] = m_queryValue;
	m_queryKey.clear();
	m_queryValue.clear();
	m_state = nextState;
}
