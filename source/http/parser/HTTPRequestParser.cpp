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

#include <encoding/default/chunk/ChunkDecoder.hpp>
#include <exception/Exception.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <util/Number.hpp>
#include <util/Optional.hpp>
#include <cctype>
#include <cstdlib>
#include <config/block/LocationBlock.hpp>
#include <libs/ft.hpp>

#if 1
#include <util/URL.hpp> /* Eclipse does not import it. */
#endif

HTTPRequestParser::HTTPRequestParser() :
		m_state(S_NOT_STARTED),
		m_method(),
		m_major(),
		m_minor(),
		m_body(""),
		m_chunkDecoder(m_body),
		m_last(),
		m_last2()
{
	m_method.reserve(16);
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
//				m_path += '/';
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
			m_state = S_END;
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
			else if (c == ' ') // ou erreur
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
		//	std:: cout << "c header: " << (int)c << std::endl;
			m_headerFieldsParser.consume(c);

			if (m_headerFieldsParser.state() == HTTPHeaderFieldsParser::S_END)
			{
	//			std::cout << "normally end\n";
				m_state = S_END;
			}
			break;
		}

		case S_BODY:
		{
			m_state = S_END;

			break;
		}

		case S_END:
		{
	//		std:: cout << "c end: " << (int)c << std::endl;
			break;
		}

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

int
HTTPRequestParser::body(std::string &storage, const Optional<DataSize> &maxBodySize)
{
	long long max;
	//std::cout << "storage: " << storage  << "  storage size : "<< storage.size()<< std::endl;
	
	if (maxBodySize.present())
		max = maxBodySize.get().toBytes();
	// else
	// 	max = -1; // can we not have maxBodySize? 
	const Optional<std::string> contentLengthOptional = headerFieldsParser().headerFields().get(HTTPHeaderFields::CONTENT_LENGTH);
	if (contentLengthOptional.present())
	{
		int bodySize = ft::atoi(contentLengthOptional.get().c_str());
		if (max != -1 && bodySize < max)
			m_body = storage.substr(0, bodySize);
		else if (max != -1)
			m_body = storage.substr(0, max);
		else
			m_body = storage;
	}
	else
	{
		const Optional<std::string> transfertEncodingOptional = headerFieldsParser().headerFields().get(HTTPHeaderFields::TRANSFER_ENCODING);
		if (transfertEncodingOptional.present())
		{
		//	std::cout << "chunk\n";
			const std::string &encoding = transfertEncodingOptional.get();
		//	ChunkDecoder chunkDecoder;
			if (encoding == "chunked" && max != -1)
			{
		//		std::cout << "first\n";
				
		
				m_body = m_chunkDecoder.decode(storage.substr(0, max));
				storage.erase(0, max);
				if (m_chunkDecoder.state() != ChunkDecoder::S_OVER)
				{
			//		std::cout << "third\n";
				return (0);
				}
			
			}
			else if (encoding == "chunked")
			{
			//	std::cout << "second\n";
		
				m_body += m_chunkDecoder.decode(storage);
		//		std::cout << "after second\n";
				storage.erase(0, max);
				if (m_chunkDecoder.state() != ChunkDecoder::S_OVER)
				{
					std::cout << "third\n";
				return (0);
				}
			}
			else
				throw Exception("transfer_encoding not supported");
		
		
		}
		else
		{
			// check with Nginx if error. RFC = content-length header SHOULD be sent...
		}
	}
	return (1);
}

URL
HTTPRequestParser::url(/*const LocationBlock *locationBlockPtr*/)
{
//	if (locationBlockPtr &&locationBlockPtr->root().present())
//	{
//		std::string path;
//
//		if (locationBlockPtr->path().size() <= m_pathParser.path().size())
//			path = m_pathParser.path().substr(locationBlockPtr->path().size(), std::string::npos);
//		else
//			path = "";
//
//		return (URL(path, m_pathParser.query(), m_pathParser.fragment()));
//	}

	return (URL(m_pathParser.path(), m_pathParser.query(), m_pathParser.fragment()));
}
