/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:51:33 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/21 17:25:53 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <encoding/default/chunk/ChunkDecoder.hpp>
#include <exception/Exception.hpp>
#include <iostream>

ChunkDecoder::ChunkDecoder() :
	m_input("input"),
	m_state(S_NOT_STARTED),
	m_sizeNb(0),
	m_parsedData(""),
	m_parsedChunk(""),
	m_extension(""),
	m_lastChar()
{
}

ChunkDecoder::ChunkDecoder(std::string input) :
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
		m_sizeNb = strtol(m_sizeStr.c_str(), &endPtr, 16);			\
		if (endPtr == m_sizeStr.c_str())							\
			throw Exception ("Hexadecimal conversion impossible"); 	\
		m_sizeStr = "";

std::string
ChunkDecoder::decode()
{
	int in_len = m_input.size();
	int in = 0;
	
	while (in_len--)
	{
		char c = m_input[in];
		in++;
		
		switch (m_state)
		{

			case S_NOT_STARTED :
			case S_SIZE:
			{
				if (ChunkDecoder::isValidCharacter(c))
				{
					m_sizeStr += c;
					m_state = S_SIZE;
				}
				else if (c == '\r')
				{
					m_state = S_SIZE_END;
					SIZE_CONVERSION();
				}
				else if (c == ';')
				{
					m_state = S_EXTENSION;
					SIZE_CONVERSION();
				}
				else
				{
					throw Exception ("Character not recognized"); // check error
				}
						
				break;
			}
			
			case S_EXTENSION:
			{
				if (c == '\r')
				{
					m_state = S_SIZE_END;
					m_extension = "";
				}
				else
				{
					m_extension += c;
					if (m_extension.size() >= 50)
						throw Exception ("Too long extensions"); // 4xx error;
				}
				
			}
			
			case S_SIZE_END:
			{
				if (c == '\n' && m_sizeNb == 0)
					m_state = S_NULL;
				else if (c == '\n')
					m_state = S_CHUNK;		
				else
					throw Exception ("\n excepted"); // check error
				
				break;
			}
			
			case S_CHUNK:
			{
				m_parsedChunk += c;
				m_sizeNb--;
				if (m_sizeNb == 0)
				{
					m_parsedData += m_parsedChunk;
					m_parsedChunk = "";
					m_sizeNb = 0;
					if (c == '\n' && m_lastChar == '\r')
						m_state = S_SIZE;
					else
						m_state = S_CHUNK_END;
				}
				else
					m_state = S_CHUNK;	

			
				break;
			}
			
			case S_CHUNK_END:
			{
				if (c == '\r')
					m_state = S_CHUNK_END2;
			
				break;
			}

			case S_CHUNK_END2:
			{
				if (c == '\n')
					m_state = S_SIZE;
				else if (c == '\r')
					m_state = S_CHUNK_END2;
				else
					m_state = S_CHUNK_END;
							
				break;
			}
			
			case S_NULL:
			{
				if (c == '\r')
					m_state = S_END;
				else
					throw Exception ("\r excepted");
				
				break;
			}
	
			case S_END:
			{
				if (c == '\n')
					m_state = S_OVER;
				else
					throw Exception ("\n excepted");
				
				break;
			}
		
			case S_OVER:
			{
				break;
			}
		}
		m_lastChar = c ;
	}
	
	return (m_parsedData);
}