/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeaderParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 11:39:57 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/17 10:54:59 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/Exception.hpp>
#include <http/header/HTTPHeaderParser.hpp>

HTTPHeaderParser::HTTPHeaderParser() :
		m_state(S_NOT_STARTED),
		m_last_char(0),
		m_last_char2(0)
{
	m_field.reserve(16); // voir pour les valeurs max
	m_value.reserve(60); // voir pour les valeurs max
}

void
HTTPHeaderParser::consume(char c) // char not possible "(),/:;<=>?@[\]{}")
{ // if "" text parsed as a single value / comments can be included by surrounding the comment text with parentheses
	switch (m_state) // pas vraiment compris l'histoire du \ -> p27 7230
	{
		case S_NOT_STARTED:
		case S_CONTINUE:
		{
			if (c == ' ')
				;
			// {
			// 	throw Exception("Space before colon"); //message a rejeter response code : 400
			// }
			else if (c == ':')
			{
				throw Exception("No Field");
			}
			else
			{
				m_state = S_FIELD; // si trop gros -> fields limit, 4xx status code
				m_field += c;
			}

			break;
		}
		case S_FIELD:
		{
			if (c == ' ')
			{
				throw Exception("Space after Field");
			}
			else if (c == ':')
			{
				m_state = S_COLON;
			}
			else
			{
				m_state = S_FIELD;
				m_field += c;
			}

			break;
		}
		case S_COLON:
		{
			if (c == ' ')
			{
				m_state = S_SPACES_BEFORE_VALUE;
			}
			else
			{
				m_state = S_VALUE;
			}
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
				m_state = S_VALUE_END;
			else if (c == '\n')
				m_state = S_VALUE_END2;
			else
				m_value += c;

			break;
		}

		case S_SPACES_AFTER_VALUE:
		{
			if (c == ' ')
				m_state = S_SPACES_AFTER_VALUE;
			else if (c == '\r')
				m_state = S_VALUE_END;
			else if (c == '\n')
				m_state = S_VALUE_END2;
			// else
			// {
			// 	throw Exception("popo");
			// }
			
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
			m_state = S_HEADER;

			break;
		}

		case S_HEADER:
		{
			if (m_last_char2 == '\n' && m_last_char == '\r' && c == '\n')
				m_state = S_END;
			else
			{
				m_state = S_CONTINUE;
			}
			

			break;
		}

		case S_END: // le compilateur veut que le mette...
		{
			break;
		}
		
	}

	m_last_char2 = m_last_char;
	m_last_char = c;
}

HTTPHeaderParser::State
HTTPHeaderParser::state() const
{
	return (m_state);
}

std::string&
HTTPHeaderParser::getField()
{
	return (m_field);
}

std::string&
HTTPHeaderParser::getValue()
{
	return (m_value);
}
