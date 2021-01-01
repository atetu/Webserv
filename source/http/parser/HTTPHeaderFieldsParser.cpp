/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeaderFieldsParser.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 21:46:29 by ecaceres          #+#    #+#             */
/*   Updated: 2020/01/01 21:46:29 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/parser/HTTPHeaderFieldsParser.hpp>

HTTPHeaderFieldsParser::HTTPHeaderFieldsParser() :
		m_state(S_FIELD),
		m_headerFields(),
		m_key(),
		m_value(),
		m_last(),
		m_last2()
{
}

void
HTTPHeaderFieldsParser::consume(char c)
{
	switch (m_state)
	{
		case S_FIELD:
		{
			if (c == ' ')
				throw Exception("Space after field");
			else if (c == ':')
				m_state = S_COLON;
			else
				m_key += c;

			break;
		}

		case S_COLON:
		{
			if (c == ' ')
				m_state = S_SPACES_BEFORE_VALUE;
			else
			{
				m_state = S_VALUE;
				m_value += c;
			}

			break;
		}

		case S_SPACES_BEFORE_VALUE:
		{
			if (c != ' ')
			{
				m_state = S_VALUE;
				m_value += c;
			}

			break;
		}

		case S_VALUE:
		{
			if (c == ' ')
				m_state = S_SPACES_AFTER_VALUE;
			else if (c == '\r')
				commit(S_VALUE_END);
			else
				m_value += c;

			break;
		}

		case S_SPACES_AFTER_VALUE:
		{
			if (c == ' ')
				m_state = S_SPACES_AFTER_VALUE;
			else if (c == '\r')
				commit(S_VALUE_END);
			else if (c == '\n')
				m_state = S_VALUE_END2;
			else
			{
				m_value += ' ';
				m_value += c;
				m_state = S_VALUE;
			}

			break;
		}

		case S_VALUE_END:
		{
			if (c == '\n')
				m_state = S_VALUE_END2;
			else
				throw Exception("Expected a \\n");

			break;
		}

		case S_VALUE_END2:
		{
			if (m_last2 == '\r' && m_last == '\n' && c == '\r')
				m_state = S_VALUE_END3;
			else
			{
				m_key += c;
				m_state = S_FIELD;
			}

			break;
		}

		case S_VALUE_END3:
		{
			if (m_last2 == '\n' && m_last == '\r' && c == '\n')
				m_state = S_END;
			else
			{
				m_key += c;
				m_state = S_FIELD; // should be an error, souldn't it?
			}

			break;
		}

		case S_END:
			break;

	}

	m_last2 = m_last;
	m_last = c;
}

void
HTTPHeaderFieldsParser::commit(State nextState)
{
	m_headerFields.set(m_key, m_value);

	m_key.clear();
	m_value.clear();

	m_state = nextState;
}
