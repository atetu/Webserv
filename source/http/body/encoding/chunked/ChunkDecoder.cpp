/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:51:33 by alicetetu         #+#    #+#             */
/*   Updated: 2021/01/11 16:53:49 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception/Exception.hpp>
#include <http/body/encoding/chunked/ChunkDecoder.hpp>
#include <cstdlib>
#include <iostream>

ChunkDecoder::ChunkDecoder(bool isAllocated) :
		m_isAllocated(isAllocated),
		m_state(S_NOT_STARTED),
		m_sizeNb(0),
		// m_parsedData(""),
		m_parsedChunk(""),
		m_extension(""),
		m_lastChar(),
		m_totalSize(0)
{
}

ChunkDecoder::~ChunkDecoder()
{
}

#define SIZE_CONVERSION()											\
		char *endPtr;												\
		std::string hex_intro = "0x" + m_sizeStr;\
		m_sizeNb = strtol(hex_intro.c_str(), &endPtr, 16);			\
		if (endPtr == hex_intro.c_str())							\
			throw Exception ("Hexadecimal conversion impossible"); 	\
		m_sizeStr = "";												\
//		std::cout << "hex: " << hex_intro << std::endl; \
//		std::cout << "nb : " << m_sizeNb << std::endl;

//std::string
//ChunkDecoder::decode(std::string storage)
//{
//	m_input += storage;
//	if (m_input.empty())
//		return (m_parsedData);
//	bool loop = true;
////	std::cout << "input:" << m_input.size() << std::endl;
//	m_totalSize += m_input.size();
////	std::cout << "total:" << m_totalSize << std::endl;
//	std::cout << "totalparsed:" << m_parsedData.size() << std::endl;
//
//	while (loop == true)
//	{
//		switch (m_state)
//		{
//
//			case S_NOT_STARTED:
//			case S_SIZE:
//			{
//				size_t found = m_input.find("\r\n");
//				if (found != std::string::npos)
//				{
//
//					m_sizeStr = m_input.substr(0, found);
//					if (m_sizeStr.empty())
//					{
//						m_state = S_OVER;
//						return (m_parsedData);
//					}
//
//					SIZE_CONVERSION();
//
//					size_t toErase = m_sizeStr.size() + (size_t)2;
//
//					m_input.erase(0, toErase);
//
//					m_sizeStr = "";
//				}
//				else
//				{
//					m_input.erase(0, std::string::npos);
//					return (m_parsedData);
//					loop = false;
//				}
//
//				if (m_sizeNb == 0)
//				{
//					//
//					m_state = S_OVER;
//					std::cout << "data size: " << m_parsedData.size() << std::endl;
//					break;
//				}
//				else
//					m_state = S_CHUNK;
//
//				break;
//			}
//
//			case S_CHUNK:
//			{
//
//				if (m_input.size() <= (size_t)m_sizeNb)
//				{
//
//					m_parsedData += m_input;
//
//					m_sizeNb -= m_input.size();
//					m_input.erase(0, std::string::npos);
//
//				}
//				else
//				{
//
//					m_parsedChunk = m_input.substr(0, m_sizeNb);
//					m_parsedData += m_parsedChunk;
//					m_input.erase(0, m_sizeNb);
//					m_parsedChunk = "";
//					m_sizeNb = 0;
//
//				}
////
//				if (m_sizeNb == 0)
//				{
//
//					m_state = S_CHUNK_END;
//					break;
//				}
//				if (!m_input.empty())
//				{
//
//					m_state = S_CHUNK;
//				}
//				else
//				{
//
//					m_state = S_CHUNK;
//					loop = false;
//					return (m_parsedData);
//				}
//
//				break;
//			}
//
//			case S_CHUNK_END:
//			{
//				size_t f = m_input.find("\r\n");
//				if (f != std::string::npos)
//				{
//
//					m_input.erase(0, f + 2);
//					m_state = S_SIZE;
//
//				}
//				else
//				{
//					// m_input.erase(0, std::string::npos);
//				}
//				if (m_input.size() == 0)
//				{
//					return (m_parsedData);
//					loop = false;
//				}
//
//				break;
//			}
//
//			case S_NULL:
//			{
//				size_t f = m_input.find("\r\n");
//				if (f != std::string::npos)
//				{
//
//					m_state = S_OVER;
//				}
//				else
//				{
//
//					m_input.erase(0, std::string::npos);
//				}
//
//				if (m_input.size() == 0)
//				{
//					loop = false;
//					return (m_parsedData);
//				}
//				break;
//			}
//
//			case S_OVER:
//			{
//				loop = false;
//
//				return (m_parsedData);
//				break;
//			}
//		}
//
//	}
//
//	return (m_parsedData);
//}

size_t
ChunkDecoder::consume(const std::string &in, std::string &out)
{
	if (in == "0\r\n\r\n")
		return (0);

	out += in[0];
	return (1);
//	//(void)out;
////	(void)c;
//
//	switch (m_state)
//	{
//		case S_NOT_STARTED :
//		case S_SIZE:
//		{
//			if (ChunkDecoder::isValidCharacter(c))
//			{
//				m_sizeStr += c;
//				m_state = S_SIZE;
//			}
//			else if (c == '\r')
//			{
//				m_state = S_SIZE_END;
//				SIZE_CONVERSION();
//			}
//			else if (c == ';')
//			{
//				m_state = S_EXTENSION;
//				SIZE_CONVERSION();
//			}
//			else
//			{
//				throw Exception ("Character not recognized"); // check error
//			}
//
//			break;
//		}
//
//		case S_EXTENSION:
//		{
//			if (c == '\r')
//			{
//				m_state = S_SIZE_END;
//				m_extension = "";
//			}
//			else
//			{
//				m_extension += c;
//				if (m_extension.size() >= 50)
//					throw Exception ("Too long extensions"); // 4xx error;
//			}
//
//			break;
//		}
//
//		case S_SIZE_END:
//		{
//			if (c == '\n' && m_sizeNb == 0)
//			{
//				//std::cout << m_parsedData << std::endl;
//			//	std::cout << "size: " << m_parsedData.size() << std::endl;
//			//	out = m_parsedData;
//				m_state = S_OVER;
//			}
//			else if (c == '\n')
//				m_state = S_CHUNK;
//			else
//				throw Exception ("\n excepted"); // check error
//
//			break;
//		}
//
//		case S_CHUNK:
//		{
//			//std::cout << "chunk: \n" << m_parsedData << std::endl;
//		//	std::cout << "size: " << m_sizeNb << std::endl;
//			out += c;
//			m_sizeNb--;
//			if (m_sizeNb == 0)
//			{
//				//m_parsedData += m_parsedChunk;
//			//	std::cout << "parsed: \n" << out << std::endl;
//			//	std::cout << "parsed: \n" << out.size() << std::endl;
//			//	m_parsedChunk = "";
//				m_sizeNb = 0;
//				m_state = S_CHUNK_END;
//			}
//			else
//				m_state = S_CHUNK;
//
//			break;
//		}
//
//		case S_CHUNK_END:
//		{
//			if (c == '\r')
//				m_state = S_CHUNK_END2;
//
//			else
//			{
//				throw Exception ("Character not recognized"); // check error
//			}
//
//			break;
//		}
//
//		case S_CHUNK_END2:
//		{
//			if (c == '\n')
//				m_state = S_SIZE;
//			else if (c == '\r')
//				m_state = S_CHUNK_END2;
//			else
//				m_state = S_CHUNK_END;
//
//			break;
//		}
//
//		case S_NULL:
//		{
//			if (c == '\r')
//				m_state = S_END;
//			else
//				throw Exception ("\r excepted");
//
//			break;
//		}
//
//		case S_END:
//		{
//			if (c == '\n')
//				m_state = S_OVER;
//			else
//				throw Exception ("\n excepted");
//
//			break;
//		}
//
//		case S_OVER:
//		{
//			//std::cout << m_parsedData << std::endl;
//			break;
//		}
//	}
//	//m_lastChar = c ;
//
//	// std::cout << "parsed: \n" << m_parsedData << std::endl;
//	return (m_state == S_OVER);
}

ChunkDecoder::State
ChunkDecoder::state()
{
	return (m_state);
}

void
ChunkDecoder::cleanup()
{
	if (m_isAllocated)
		delete this;
}
