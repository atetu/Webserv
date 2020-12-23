/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkEncoder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:51:33 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/23 18:29:30 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <encoding/default/chunk/ChunkEncoder.hpp>
#include <exception/Exception.hpp>
#include <iomanip>
#include <sstream>

ChunkEncoder::ChunkEncoder() :
	m_input(""),
	m_parsedData("")
{
}

ChunkEncoder::ChunkEncoder(std::string &input) :
	m_input(input),
	m_parsedData("")
{
}

ChunkEncoder::ChunkEncoder(const ChunkEncoder &other) :
	m_input(other.m_input),
	m_parsedData(other.m_parsedData)
{
}

ChunkEncoder::~ChunkEncoder()
{
}

ChunkEncoder&
ChunkEncoder::operator =(const ChunkEncoder &other)
{
	(void)other;

	return (*this);
}

std::string
ChunkEncoder::encode()
{
	while (!m_input.empty())
	{
		std::string chunkStr = m_input.substr(0, 50);
		int chunkSizeNb = chunkStr.size();
		std::stringstream stream;
		stream << std::hex << chunkSizeNb;
		std::string chunkSikeStr(stream.str());
		std::string chunkToAdd = chunkSikeStr + "\r\n" + chunkStr + "\r\n";

		m_parsedData += chunkToAdd;
		
		m_input.erase(0, 48);
	}
	m_parsedData += "0\r\n\r\n";
	
	return (m_parsedData);
}
