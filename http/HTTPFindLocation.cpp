/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPFindLocation.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 15:24:50 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/15 17:36:18 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTPFindLocation.hpp>
#include <http/HTTPLocationInterpretor.hpp>

HTTPFindLocation::HTTPFindLocation() :
		m_clientPath(""),
		m_serverLocations(),
		m_locationBlock()
{
}

HTTPFindLocation::HTTPFindLocation(std::string clientPath, std::vector<LocationBlock*> serverLocations) :
		m_clientPath(clientPath),
		m_serverLocations(serverLocations),
		m_locationBlock()
{
}

HTTPFindLocation::HTTPFindLocation(const HTTPFindLocation &other) :
		m_clientPath(other.m_clientPath),
		m_serverLocations(other.m_serverLocations),
		m_locationBlock(other.m_locationBlock)
{
}

HTTPFindLocation::~HTTPFindLocation()
{
}

HTTPFindLocation&
HTTPFindLocation::operator =(const HTTPFindLocation &other)
{
	if (this != &other)
	{
		m_clientPath = other.m_clientPath;
		m_serverLocations = other.m_serverLocations;
		m_locationBlock = other.m_locationBlock;
	}
	return (*this);
}

LocationBlock*
HTTPFindLocation::location(void)
{
	return (m_locationBlock);
}



void
HTTPFindLocation::parse(void)
{
	std::vector<LocationBlock*>::iterator it = m_serverLocations.begin();
	std::vector<LocationBlock*>::iterator ite = m_serverLocations.end();

	while (it != ite)
	{
		HTTPLocationInterpretor interpretor((*it)->path());
		
		char c ;
	
		while (interpretor.next(c))
			interpretor.consume(c);
//		interpretor.consume('\0');

		if(!(interpretor.begin().empty()))
		{
			if (!(interpretor.begin().compare(0, m_clientPath.size(), m_clientPath) == 0))
				break;
		}
		if(!(interpretor.middle().empty()))
		{
			
		}
		if(!(interpretor.end().empty()))
		{
			
		}
		
		it ++;
	}
}
