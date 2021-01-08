/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:51:33 by alicetetu         #+#    #+#             */
/*   Updated: 2021/01/07 14:37:56 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <encoding/default/chunk/ChunkDecoder.hpp>
#include <exception/Exception.hpp>
#include <cstdlib>
#include <iostream>

ChunkDecoder::ChunkDecoder() :
	m_input(""),
	m_state(S_NOT_STARTED),
	m_sizeNb(0),
	// m_parsedData(""),
	m_parsedChunk(""),
	m_extension(""),
	m_lastChar(),
	m_totalSize(0)
{
	//m_parsedData.reserve(10000000);
}

// ChunkDecoder::ChunkDecoder(std::string &body) :
// 	m_input(""),
// 	m_state(S_NOT_STARTED),
// 	m_sizeNb(0),
// 	m_parsedData(body),
// 	m_parsedChunk(""),
// 	m_extension(""),
// 	m_lastChar()
// {
// }
ChunkDecoder::ChunkDecoder(const std::string &input) :
	m_input(input),
	m_state(S_NOT_STARTED),
	m_sizeNb(0),
	m_parsedData(""),
	m_parsedChunk(""),
	m_extension(""),
	m_lastChar()
{
}

ChunkDecoder::ChunkDecoder(const ChunkDecoder &other) :
	m_input(other.m_input),
	m_state(other.m_state),
	m_sizeNb(other.m_sizeNb),
	m_parsedData(other.m_parsedData),
	m_parsedChunk(other.m_parsedChunk),
	m_extension(other.m_extension),
	m_lastChar(other.m_lastChar)
{
}

ChunkDecoder::~ChunkDecoder()
{
}

ChunkDecoder&
ChunkDecoder::operator =(const ChunkDecoder &other)
{
	m_input = other.m_input;
	m_state = other.m_state;
	m_sizeNb = other.m_sizeNb;
	m_parsedData = other.m_parsedData;
	m_parsedChunk = other.m_parsedChunk;
	m_extension = other.m_extension;
	m_lastChar = other.m_lastChar;

	return (*this);
}

#define SIZE_CONVERSION()											\
		char *endPtr;												\
		std::string hex_intro = "0x" + m_sizeStr;\
		m_sizeNb = strtol(hex_intro.c_str(), &endPtr, 16);			\
		if (endPtr == hex_intro.c_str())							\
			throw Exception ("Hexadecimal conversion impossible"); 	\
		std::cout << "hex: " << hex_intro << std::endl; \
		std::cout << "nb : " << m_sizeNb << std::endl;
	
std::string
ChunkDecoder::decode(std::string storage)
{
	m_input += storage;
	if (m_input.empty())
		return(m_parsedData);
	bool loop = true;
//	std::cout << "input:" << m_input.size() << std::endl;
	m_totalSize += m_input.size();
//	std::cout << "total:" << m_totalSize << std::endl;
		std::cout << "totalparsed:" << m_parsedData.size() << std::endl;

	while(loop == true)
	{
 		switch (m_state)
		{
 		
			case S_NOT_STARTED :
			case S_SIZE:
			{
				size_t found = m_input.find("\r\n");
				if (found != std::string::npos)
				{
				
					m_sizeStr = m_input.substr(0, found);
					if (m_sizeStr.empty())
					{
						m_state = S_OVER;
						return (m_parsedData);
					}
					
					SIZE_CONVERSION();
				
 					
					size_t toErase = m_sizeStr.size() + (size_t)2;
				
					m_input.erase(0, toErase);
				
					m_sizeStr = "";
				}
				else
				{
					m_input.erase(0, std::string::npos);
					return(m_parsedData);
					loop = false;
				}
				
				if (m_sizeNb == 0)
 				{
 	//				
 					m_state = S_OVER;
					 std::cout << "data size: "<< m_parsedData.size() << std::endl;
					 break;
 				}
				 else
 						m_state = S_CHUNK;
				
				break;
			}
			
 			case S_CHUNK:
 			{
 		
 				if (m_input.size() <= (size_t)m_sizeNb)
 				{
 				
 					m_parsedData += m_input;
			
					m_sizeNb -= m_input.size();
					m_input.erase(0, std::string::npos);
				
 				}
				else
				{
		
					m_parsedChunk = m_input.substr(0, m_sizeNb);
					m_parsedData += m_parsedChunk;
					m_input.erase(0, m_sizeNb);
					m_parsedChunk = "";
					m_sizeNb = 0;
		
				}
// 		
				if (m_sizeNb == 0)
 				{
		
 					m_state = S_CHUNK_END;
					 break;
 				}
 				if (!m_input.empty())
				 {
				
 					m_state = S_CHUNK;	
				 }
				else
				{
				
					m_state = S_CHUNK;
					loop = false;
					return(m_parsedData);
				}
				
	
 				break;
 			}
			
 			case S_CHUNK_END:
 			{
				size_t f = m_input.find("\r\n");
				if (f != std::string::npos)
				{
			
					m_input.erase(0, f + 2);
					m_state = S_SIZE;
			
				}
				else
				{
					// m_input.erase(0, std::string::npos);
				}
				if (m_input.size() == 0)
				{
					return(m_parsedData);
					loop = false;
				}

				break;
			 }
				
				 
 		
			
 			case S_NULL:
 			{
				size_t f = m_input.find("\r\n");
				if (f != std::string::npos)
				{
				
					m_state = S_OVER;
				}
				else
				{
				
					m_input.erase(0, std::string::npos);
				}

				if (m_input.size() == 0)
				{
					loop = false;
					return(m_parsedData);
			 	}
				break;
			 }
 
 			case S_OVER:
 			{
 				loop = false;
	
 				return (m_parsedData);
 				break;
 			}
 		}

	}

	

return (m_parsedData);
}

ChunkDecoder::State 
ChunkDecoder::state()
{
	return (m_state);
}
