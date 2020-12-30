/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:29:02 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 14:33:28 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <encoding/default/chunk/ChunkDecoder.hpp>
#include <exception/Exception.hpp>
#include <http/request/parser/HTTPRequestParser.hpp>
#include <util/Number.hpp>
#include <util/Optional.hpp>
#include <cctype>
#include <cstdlib>

#if 1
#include <util/URL.hpp> /* Eclipse does not import it. */
#endif

# ifdef major
#  undef major
# endif

# ifdef minor
#  undef minor
# endif

HTTPRequestParser::HTTPRequestParser() :
		m_state(S_NOT_STARTED),
		m_method(),
		m_path(),
		m_major(),
		m_minor(),
		m_field(),
		m_value(),
		m_headerFields(),
		m_query(),
		m_queryKey(),
		m_queryValue(),
		m_fragment(),
		m_hexBeforeState(),
		m_hexStoreTarget(),
		m_hex(),
		m_last_char(),
		m_last_char2()
{
	m_method.reserve(16);
	m_path.reserve(60);
}

#define ADD_TO_MAP(key, value)						\
			m_headerFields.set(key, value);			\
			key = "";								\
			value = "";								

#define ADD_TO_QUERY_MAP(stateType)												\
			{																	\
				m_state = stateType;											\
				if (m_queryKey.empty() || m_queryValue.empty())					\
					throw Exception ("Query keys and values cannot be empty");	\
				m_query[m_queryKey] = m_queryValue;								\
				m_queryKey = "";												\
				m_queryValue = "";												\
			}

#define HEX_CONVERSION(c, str)													\
			{																	\
				if (isalnum(c))													\
					{															\
						m_hex += c;												\
						if (m_hex.size() == 2)									\
						{														\
							char *p;											\
							long n = strtol(m_hex.c_str(), &p, 16);				\
							if (*p != 0)										\
								throw Exception("Conversion not allowed");		\
							str += (char)n;										\
							m_hex = "";											\
							m_hexOn = false;									\
						}														\
					}															\
				else															\
						throw Exception("Wrong conversion");					\
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
				if (!::isupper(c))
					throw Exception("Method is only upper-case letter");

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
				m_path += '/';
			}

			break;
		}

		case S_PATH:
		{
			if (c == ' ')
				m_state = S_HTTP_START;
			else if (c == '%')
				hexStart(&m_path);
			else if (c == '?')
				m_state = S_QUERY_STRING_KEY;
			else if (c == '#')
				m_state = S_FRAGMENT;
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
//			else if (c == '+')
//				m_queryKey += ' '; // TODO @atetu can you explain???
			else if (c == ' ')
				m_state = S_HTTP_START;
			else
				m_queryKey += c;

			break;
		}

		case S_QUERY_STRING_VALUE:
		{
			if (c == '%')
				hexStart(&m_queryValue);
			else if (c == '&')
				ADD_TO_QUERY_MAP(S_QUERY_STRING_KEY)
			else if (c == '#')
				ADD_TO_QUERY_MAP(S_FRAGMENT)
			else if (c == ' ')
				ADD_TO_QUERY_MAP(S_HTTP_START)
			else
				m_queryValue += c;

			break;
		}

		case S_FRAGMENT:
		{
			if (c == ' ')
				m_state = S_HTTP_START;
			else if (c == '%')
				hexStart(&m_fragment);
			else
				m_fragment += c;

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
			if (!::isdigit(c))
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
			if (!::isdigit(c))
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
			m_state = S_END;
			if (m_last_char2 == '\r' && m_last_char == '\n' && c == '\r')
				m_state = S_HTTP_END3;
			else if (c == ' ')
				throw Exception("Space before field");
			else
			{
				m_state = S_FIELD;
				m_field += c;
			}

			break;
		}

		case S_HTTP_END3:
		{
			if (m_last_char2 == '\n' && m_last_char == '\r' && c == '\n')
				m_state = S_END;
			else if (c == ' ') // ou erreur
				throw Exception("Space before field");
			else
			{
				m_state = S_FIELD;
				m_field += c;
			}

			break;
		}

		case S_FIELD:
		{
			if (c == ' ')
				throw Exception("Space after Field");
			else if (c == ':')
				m_state = S_COLON;
			else
				m_field += c;

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
			{
				ADD_TO_MAP(m_field, m_value);
				m_state = S_VALUE_END;
			}
			else
				m_value += c;

			break;
		}

		case S_SPACES_AFTER_VALUE:
		{
			if (c == ' ')
				m_state = S_SPACES_AFTER_VALUE;
			else if (c == '\r')
			{

				ADD_TO_MAP(m_field, m_value);
				m_state = S_VALUE_END;
			}
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
			if (m_last_char2 == '\r' && m_last_char == '\n' && c == '\r')
				m_state = S_VALUE_END3;
			else
			{
				m_field += c;
				m_state = S_FIELD;
			}

			break;
		}

		case S_VALUE_END3:
		{
			if (m_last_char2 == '\n' && m_last_char == '\r' && c == '\n')
				m_state = S_END;
			else
			{
				m_field += c;
				m_state = S_FIELD; // should be an error, souldn't it?
			}

			break;
		}

		case S_END:
			break;

	}

//	std::cout << m_state << " -- " << c << std::endl;

	m_last_char2 = m_last_char;
	m_last_char = c;
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

std::string
HTTPRequestParser::path() const
{
	return (m_path);
}

int
HTTPRequestParser::major() const
{
	return (m_major);
}

int
HTTPRequestParser::minor() const
{
	return (m_minor);
}

const HTTPHeaderFields&
HTTPRequestParser::headerFields()
{
	return (m_headerFields);
}

char
HTTPRequestParser::lastChar() const
{
	return (m_last_char);
}

std::map<std::string, std::string>&
HTTPRequestParser::query()
{
	return (m_query);
}

std::string&
HTTPRequestParser::fragment()
{
	return (m_fragment);
}

void
HTTPRequestParser::body(const std::string &storage)
{
	m_body = storage;
}

const std::string&
HTTPRequestParser::body() const
{
	return (m_body);
}

void
HTTPRequestParser::body(std::string &storage)
{
	const Optional<std::string> contentLengthOptional = m_headerFields.get(HTTPHeaderFields::CONTENT_LENGTH);
	if (contentLengthOptional.present())
	{
		int bodySize = ::atoi(contentLengthOptional.get().c_str());
		m_body = storage.substr(0, bodySize);
	}
	else
	{
		const Optional<std::string> transfertEncodingOptional = m_headerFields.get(HTTPHeaderFields::TRANSFER_ENCODING);
		if (transfertEncodingOptional.present())
		{
			const std::string &encoding = transfertEncodingOptional.get();

			if (encoding == "chunked")
				m_body = ChunkDecoder(storage).decode();
			else
				throw Exception("transfer_encoding not supported");
		}
		else
		{
			// check with Nginx if error. RFC = content-length header SHOULD be sent...
		}
	}
}

URL
HTTPRequestParser::url()
{
	const Optional<std::map<std::string, std::string> > queryMap = Optional<std::map<std::string, std::string> >::ofEmpty(query());
	const Optional<std::string> fragmentStr = Optional<std::string>::ofEmpty(fragment());

	return (URL(path(), queryMap, fragmentStr));
}
