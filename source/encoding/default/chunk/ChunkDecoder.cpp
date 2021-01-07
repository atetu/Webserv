/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:51:33 by alicetetu         #+#    #+#             */
/*   Updated: 2021/01/07 12:18:21 by alicetetu        ###   ########.fr       */
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
	m_parsedData(""),
	m_parsedChunk(""),
	m_extension(""),
	m_lastChar(),
	m_totalSize(0)
{
}

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
	m_input = storage;
	if (m_input.empty())
		return(m_parsedData);
	bool loop = true;
	std::cout << "input:" << m_input.size() << std::endl;
	m_totalSize += m_input.size();
	std::cout << "total:" << m_totalSize << std::endl;
//	size_t found = m_input.find("\r\n");
	
// 	// if (m_input == "\r\n")
// 	// 	return (m_parsedData);
// 	m_input = storage;
// //	std::cout << "decode\n";
// 	int in_len = m_input.size();
// 	std::cout << "len: " << in_len << std::endl;
// 	int in = 0;
// 	//std::cout << m_state << "\n";
// 	while (in_len-- && m_state != S_OVER)
// 	{
// 		char c = m_input[in];
// 		in++;
// 	//	std::cout << m_state << "\n";
	while(loop == true)
	{
 		switch (m_state)
		{
 			std::cout << m_state << "\n";
			case S_NOT_STARTED :
			case S_SIZE:
			{
				size_t found = m_input.find("\r\n");
				if (found != std::string::npos)
				{
					std::cout << "found / " << m_input[found]  << m_input[found + 1]<< std::endl;
					m_sizeStr = m_input.substr(0, found);
					if (m_sizeStr.empty())
					{
						m_state = S_OVER;
						return (m_parsedData);
					}
					std::cout << "size: " << m_sizeStr << std::endl;
					SIZE_CONVERSION();
				
 					
					std::cout << "input size bef: " << m_input.size() << std::endl;
					size_t toErase = m_sizeStr.size() + (size_t)2;
					std::cout << "to erase: " << toErase << std::endl;
					m_input.erase(0, toErase);
					std::cout << "input size after: " << m_input.size() << std::endl;
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
 	//				m_parsedData += m_parsedChunk;
 //					std::cout << "parsed: " << m_parsedData << std::endl;
// 					std::cout << "chunk: " << m_parsedChunk << std::endl;
// 					std::cout << "chunksize: " << m_parsedData.size() << std::endl;
// 					m_parsedChunk = "";
// 					m_sizeNb = 0;
 					m_state = S_OVER;
					 break;
 				}
				 else
 						m_state = S_CHUNK;
				//std::cout << "input sec:"<< m_input.size() << std::endl;
				//std::cout << "pasred:" << m_parsedData.size() << std::endl;
				break;
			}
			//	std::cout << "c: " << c << "\n";
			//	if (ChunkDecoder::isValidCharacter(c))
			//	{
// 					m_sizeStr += c;
// 					m_state = S_SIZE;
// 				}
// 				else if (c == '\r')
// 				{
// 					m_state = S_SIZE_END;
// 					// SIZE_CONVERSION();
// 				}
// 				else if (c == ';')
// 				{
// 					m_state = S_EXTENSION;
// 					SIZE_CONVERSION();
// 				}
// 				else
// 				{
// 					std::cout << "characetr not recognized\n";
// 					throw Exception ("Character not recognized"); // check error
// 				}
						
// 				break;
// 			}
			
// 			case S_EXTENSION:
// 			{
// 			//	std::cout << m_state << "\n";
// 				if (c == '\r')
// 				{
// 					m_state = S_SIZE_END;
// 					m_extension = "";
// 				}
// 				else
// 				{
// 					m_extension += c;
// 					if (m_extension.size() >= 50)
// 						throw Exception ("Too long extensions"); // 4xx error;
// 				}
				
// 				break;
// 			}
			
// 			case S_SIZE_END:
// 			{
// 				std::cout << m_state ;
// 				if (c == '\n')
// 				{
// 					std::cout << " sizeSTR :"<<  m_sizeStr<< " Size: " << m_sizeStr.size()<<std::endl;
// 					size_t sizeHexa =2;
// 					if (m_sizeStr.size() > sizeHexa)
// 					{
// 						std::cout << "sub\n";
// 						size_t newpos= m_sizeStr.size() - 2;
// 						std::cout << "new pos: " << newpos << std::endl;
// 						m_sizeStr.substr(newpos, std::string::npos);
// 					}
// 					std::cout << " size:"<<  m_sizeStr<< std::endl;
// 					SIZE_CONVERSION();
// 					if (m_sizeNb == 0)
// 						m_state = S_NULL;
// 					else
// 						m_state = S_CHUNK;		
// 				}
// 				else
// 				{
// 					m_state = S_SIZE;
// 				}
				
// 				break;
// 			}
			
 			case S_CHUNK:
 			{
 			//	std::cout << m_state ;
 				//std::cout << "chunk: \n" << m_parsedChunk << std::endl;
 			//	std::cout << "size: " << m_sizeNb << std::endl;
 				if (m_input.size() <= (size_t)m_sizeNb)
 				{
 				//	m_parsedChunk = m_input.substr(in, std::string::npos);
 				//	m_sizeNb -= m_parsedChunk.size();
 					m_parsedData += m_input;
					m_sizeNb -= m_input.size();
					m_input.erase(0, std::string::npos);
					std::cout << "input erase totally: " << m_input << std::endl;
					std::cout << "sed\n";
 					//	std::cout << "parsed: " << m_parsedData << std::endl;
 				//	std::cout << "chunk: " << m_parsedChunk << std::endl;
 				//	std::cout << "chunksize: " << m_parsedChunk.size() << std::endl;
 				//	std::cout << "parsedsize: " << m_parsedData.size() << std::endl;
 				//	std::cout << "size NB: " << m_sizeNb << std::endl;
 				//	m_parsedChunk = "";
 				//	in_len = 0;
					
 				}
				else
				{
					std::cout << "sizeNb: " << m_sizeNb << std::endl;
					std::cout << "input size: " << m_input.size() << std::endl;
					m_parsedChunk = m_input.substr(0, m_sizeNb);
					m_parsedData += m_parsedChunk;
					m_input.erase(0, m_sizeNb);
					m_parsedData = "";
					m_sizeNb = 0;
					std::cout << "input erase partially: " << m_input << std::endl;
				}
// 				else
// 				{		
// 					m_parsedChunk += c;
// 					m_sizeNb--;
// 				}
 				if (m_sizeNb == 0)
 				{
 	//				m_parsedData += m_parsedChunk;
 //					std::cout << "parsed: " << m_parsedData << std::endl;
// 					std::cout << "chunk: " << m_parsedChunk << std::endl;
// 					std::cout << "chunksize: " << m_parsedData.size() << std::endl;
// 					m_parsedChunk = "";
// 					m_sizeNb = 0;
 					m_state = S_CHUNK_END;
					 break;
 				}
 				if (!m_input.empty())
				 {
					// std::cout << "inut: " << m_input << std::endl;
 					m_state = S_CHUNK;	
				 }
				else
				{
					//std::cout << "false\n";
					m_state = S_CHUNK;
					loop = false;
					return(m_parsedData);
				}
				
		//	std::cout << "parsed: " << m_parsedData << " - state : " << m_state << std::endl;
 				break;
 			}
			
 			case S_CHUNK_END:
 			{
				size_t f = m_input.find("\r\n");
				if (f != std::string::npos)
				{
					std::cout << "input end" << m_input << std::endl;
					std::cout << "chunkend\n";
					m_input.erase(0, f + 2);
					m_state = S_SIZE;
				}
				else
				{
					m_input.erase(0, std::string::npos);
				}
				if (m_input.size() == 0)
				{
					return(m_parsedData);
					loop = false;
				}

				break;
			 }
				
				 
 			//	std::cout << m_state;
// 				if (c == '\r')
// 					m_state = S_CHUNK_END2;
// 				// else if (c == '\n')
// 				// 	m_state = S_SIZE;
// 				else
// 				{
// 					m_state = S_CHUNK_END;
// 					// std::cout << "c : " << c << std::endl;
// 					// std::cout << "error chunk end\n";
// 					// // throw Exception ("Character not recognized"); // check error
// 					// m_state = S_SIZE;
// 					// m_sizeStr += c;
// 				}
		
// 				break;
// 			}

// 			case S_CHUNK_END2:
// 			{
// 				std::cout << m_state ;
// 				if (c == '\n')
// 					m_state = S_SIZE;
// 				else if (c == '\r')
// 					m_state = S_CHUNK_END2;
// 				else
// 					m_state = S_CHUNK_END;
							
// 				break;
// 			}
			
 			case S_NULL:
 			{
				size_t f = m_input.find("\r\n");
				if (f != std::string::npos)
				{
					std::cout << "over\n";
					m_state = S_OVER;
				}
				else
				{
					std::cout << " earse \n";
					m_input.erase(0, std::string::npos);
				}

				if (m_input.size() == 0)
				{
					loop = false;
					return(m_parsedData);
			 	}
				break;
			 }
 //				std::cout << m_state ;
// 				if (c == '\r')
// 					m_state = S_END;
// 				else if (c == '\n')
// 				 	m_state = S_OVER;
// 				else
// 					m_state = S_NULL;
				
// 				break;
// 			}
	
// 			case S_END:
// 			{
// 				std::cout << m_state;
// 				if (c == '\n')
// 					m_state = S_OVER;
// 				else
// 					m_state = S_END;
// 				//	throw Exception ("\n excepted");
				
// 				break;
// 			}
		
 			case S_OVER:
 			{
 				loop = false;
				 std::cout << "loop" << loop << std::endl;
 				return (m_parsedData);
 				break;
 			}
 		}
// 		m_lastChar = c ;
	}
//	std::cout << "parsed: \n" << m_parsedData << std::endl;
	

return (m_parsedData);
}

ChunkDecoder::State 
ChunkDecoder::state()
{
	return (m_state);
}
