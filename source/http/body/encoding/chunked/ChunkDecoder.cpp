/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:51:33 by alicetetu         #+#    #+#             */
/*   Updated: 2021/01/13 16:16:55 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <exception/Exception.hpp>
#include <http/body/encoding/chunked/ChunkDecoder.hpp>
#include <cstdlib>
#include <iostream>
#include <config/Configuration.hpp>

ChunkDecoder::ChunkDecoder(bool isAllocated) :
		m_isAllocated(isAllocated),
		m_state(S_NOT_STARTED),
		m_sizeNb(0),
		m_sizeStr(""),
		m_parsedChunk(""),
		m_totalSize(0)
{
}

ChunkDecoder::~ChunkDecoder()
{
}

#define SIZE_CONVERSION()											\
		char *endPtr;												\
		std::string hex_intro = "0x" + m_sizeStr;					\
		m_sizeNb = strtol(hex_intro.c_str(), &endPtr, 16);			\
		if (endPtr == hex_intro.c_str())							\
			throw Exception ("Hexadecimal conversion impossible"); 	\
		m_sizeStr = "";												
		// std::cout << "hex: " << hex_intro << std::endl; 			
		// std::cout << "nb : " << m_sizeNb << std::endl;

bool
ChunkDecoder::consume(const std::string &in, std::string &out, size_t &consumed)
{
std::cout << "IN: " << in.size() << std::endl;
	switch (m_state)
	{
		case S_NOT_STARTED:
		case S_SIZE:
		{
		//	std::cout << "size: "<< in << std::endl;
			size_t found;
			found = in.find("\r\n");
			if (found != std::string::npos)
			{
				m_sizeStr = in.substr(0, found);
			//	std::cout << "SIZE 1: "<< m_sizeStr << std::endl;
				consumed += found + 2;
			//	std::cout << "Consumed: "<< consumed << std::endl;
				if (m_sizeStr.empty())
				{
					m_state = S_OVER;
					return (true);
				}

				SIZE_CONVERSION();
			//	std::cout << "SIZE: "<< m_sizeNb << std::endl;
				m_sizeStr = "";
			}
			else
			{
			
				return (false);
			}

			if (m_sizeNb == 0)
			{
					//
				m_state = S_OVER;
			//	std::cout << "over\n";
				return (true);
				// std::cout << "data size: " << m_parsedData.size() << std::endl;
				// break;
			}
			else
			{
				m_state = S_CHUNK;
			//	std::cout << "chunk state\n";
				
			}
			break;
		}

		case S_CHUNK:
		{
			size_t fn;
			fn = in.find("\r\n");
						
			if (fn != std::string::npos)
			{
				//	std::cout << "found\n";
			}
	
			if (in.size() <= (size_t)m_sizeNb)
			{
		
				out += in;
				m_sizeNb -= in.size();
				consumed += in.size();
				
			}
			else
			{
				m_parsedChunk = in.substr(0, m_sizeNb);
				out += m_parsedChunk;
				consumed += m_sizeNb;
				m_parsedChunk = "";
				m_sizeNb = 0;
		
			}

			if (m_sizeNb == 0)
			{
				m_state = S_CHUNK_END;
			}
			
			// std::cout << "sizeNB: " << m_sizeNb << std::endl;
			// std::cout << "consumed: " << consumed << std::endl;
			// std::cout << "out: " << out.size() << std::endl;
			// std::cout << "state: " << m_state << std::endl;
			break;
		}

		case S_CHUNK_END:
		{
		//	std::cout << "IBN:" ;
			size_t f;
		//	std::cout << "size: " << (int)in[0] << std::endl;;
			f = in.find("\r\n");
			if (f != std::string::npos)
			{
		//		std::cout << "chunk end\n";
				consumed += f + 2;
				//m_input.erase(0, f + 2);
				m_state = S_SIZE;
			}
			else if ((f = in.find("\r")) != std::string::npos)
			{
				consumed += f + 1;
				m_state = S_CHUNK_END2;
			}
			else
			{
				return (true);
			}
				// if (in.size() == 0)
				// {
				// 	return (true);
				// //	loop = false;
				// }

			break;
		}
		case S_CHUNK_END2:
		{
		//	std::cout << "IBN:" ;
			size_t f;
		//	std::cout << "size: " << (int)in[0] << std::endl;;
			f = in.find("\n");
			if (f != std::string::npos)
			{
			//	std::cout << "chunk end\n";
				consumed += f + 1;
				//m_input.erase(0, f + 2);
				m_state = S_SIZE;
			}
			else if (in.size() != 0)
			{
				m_state = S_CHUNK_END;
			}
				
			break;
		}	
	
		case S_OVER:
		{
			return (true);
		}
	}

	
	
	

	return (false);
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
