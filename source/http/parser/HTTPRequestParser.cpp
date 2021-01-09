/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:29:02 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/08 14:42:03 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/Exception.hpp>
#include <http/body/encoding/HTTPBodyEncoding.hpp>
#include <http/body/encoding/IHTTPBodyDecoder.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <libs/ft.hpp>
#include <util/Optional.hpp>

#if 1
#include <util/URL.hpp> /* Eclipse does not import it. */
#endif

HTTPRequestParser::HTTPRequestParser(std::string &body) :
		m_state(S_NOT_STARTED),
		m_method(),
		m_major(),
		m_minor(),
		m_last(),
		m_last2(),
		m_body(body),
		m_bodyDecoder()
{
	m_method.reserve(16);
}

HTTPRequestParser::~HTTPRequestParser()
{
	if (m_bodyDecoder)
		m_bodyDecoder->cleanup();
}

void
HTTPRequestParser::consume(char c)
{
	switch (m_state)
	{
		case S_NOT_STARTED:
		case S_METHOD:
		{
			if (c == ' ')
			{
				if (m_state == S_NOT_STARTED)
					throw Exception("No method");

				m_state = S_SPACES_BEFORE_PATH;
			}
			else
			{
				if (!ft::isupper(c))
					throw Exception("Method is only upper-case letter");

				if (m_method.length() > 20)
					throw Exception("Method is over 20 characters");

				m_state = S_METHOD;
				m_method += c;
			}

			break;
		}

		case S_SPACES_BEFORE_PATH:
		{
			if (c != ' ')
			{
				if (c != '/')
					throw Exception("No slash");

				m_state = S_PATH;
				// m_path += '/'; /* Added in the constructor of the URL object. */
			}

			break;
		}

		case S_PATH:
		{
			m_pathParser.consume(c);

			if (m_pathParser.state() == HTTPRequestPathParser::S_END)
				m_state = S_HTTP_START;

			break;
		}

		case S_HTTP_START:
		{
			if (c != 'H')
				throw Exception("Expected `H`");

			m_state = S_HTTP_H;

			break;
		}

		case S_HTTP_H:
		{
			if (c != 'T')
				throw Exception("Expected `T`");

			m_state = S_HTTP_HT;

			break;
		}

		case S_HTTP_HT:
		{
			if (c != 'T')
				throw Exception("Expected `T`");

			m_state = S_HTTP_HTT;

			break;
		}

		case S_HTTP_HTT:
		{
			if (c != 'P')
				throw Exception("Expected `P`");

			m_state = S_HTTP_HTTP;

			break;
		}

		case S_HTTP_HTTP:
		{
			if (c != '/')
				throw Exception("Expected `/`");

			m_state = S_HTTP_SLASH;

			break;
		}

		case S_HTTP_SLASH:
		{
			if (!ft::isdigit(c))
				throw Exception("Expected a number");

			m_major = c - '0';
			m_state = S_HTTP_MAJOR;

			break;
		}

		case S_HTTP_MAJOR:
		{
			if (c != '.')
				throw Exception("Expected `.`");

			m_state = S_HTTP_DOT;

			break;
		}

		case S_HTTP_DOT:
		{
			if (!ft::isdigit(c))
				throw Exception("Expected a number");

			m_minor = c - '0';
			m_state = S_HTTP_MINOR;

			break;
		}

		case S_HTTP_MINOR:
		{
			if (c == '\r')
				m_state = S_HTTP_END;
			else if (c == '\n')
				m_state = S_HTTP_END2; // TODO Changed since the parser hang forever. //S_HTTP_END2;
			else
				throw Exception("Expected a \\r or \\n");

			break;
		}

		case S_HTTP_END:
		{
			if (c == '\n')
				m_state = S_HTTP_END2; // TODO Changed since the parser hang forever. //S_HTTP_END2;
			else
				throw Exception("Expected a \\n");

			break;
		}

		case S_HTTP_END2:
		{
			m_state = S_END; // FIXME
			if (m_last2 == '\r' && m_last == '\n' && c == '\r')
				m_state = S_HTTP_END3;
			else if (c == ' ')
				throw Exception("Space before field");
			else
			{
				m_state = S_HEADER_FIELDS;
				m_headerFieldsParser.consume(c);
			}

			break;
		}

		case S_HTTP_END3:
		{
			if (m_last2 == '\n' && m_last == '\r' && c == '\n')
				m_state = S_END;
			else if (c == ' ')
				throw Exception("Space before field");
			else
			{
				m_state = S_HEADER_FIELDS;
				m_headerFieldsParser.consume(c);
			}

			break;
		}

		case S_HEADER_FIELDS:
		{
			m_headerFieldsParser.consume(c);

			if (m_headerFieldsParser.state() == HTTPHeaderFieldsParser::S_END)
				m_state = S_END;

			break;
		}

		case S_BODY:
			m_state = S_BODY_DECODE;
			m_bodyDecoder = &HTTPBodyEncoding::decoderFor(m_headerFieldsParser.headerFields());

			/* Falling */

		case S_BODY_DECODE:
		{
			if (m_bodyDecoder->consume(m_body, c))
				m_state = S_END;

			break;
		}

		case S_END:
			break;

	}

//	std::cout << m_state << " -- " << c << std::endl;
//	std::cout << c << std::flush;

	m_last2 = m_last;
	m_last = c;
}

HTTPRequestParser::State&
HTTPRequestParser::state()
{
	return (m_state);
}

HTTPRequestParser::State
HTTPRequestParser::state() const
{
	return (m_state);
}

std::string
HTTPRequestParser::method() const
{
	return (m_method);
}

const std::string&
HTTPRequestParser::body() const
{
	return (m_body);
}

URL
HTTPRequestParser::url()
{
	return (URL(m_pathParser.path(), m_pathParser.query(), m_pathParser.fragment()));
}
