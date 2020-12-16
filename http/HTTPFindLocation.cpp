/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPFindLocation.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 15:24:50 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/16 15:46:27 by alicetetu        ###   ########.fr       */
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

HTTPFindLocation::HTTPFindLocation(std::string clientPath, std::list<const LocationBlock*> serverLocations) :
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

HTTPFindLocation&
HTTPFindLocation::location(const LocationBlock* locationBlock)
{
	m_locationBlock.set(locationBlock);

	return (*this);
}
		
		
	
const Optional<const LocationBlock*>
HTTPFindLocation::location(void) const
{
	return (m_locationBlock);
}



void
HTTPFindLocation::parse(void)
{
	std::list<const LocationBlock*>::iterator it = m_serverLocations.begin();
	std::list<const LocationBlock*>::iterator ite = m_serverLocations.end();

	while (it != ite)
	{
		HTTPLocationInterpretor interpretor((*it)->path());
		
		
		
		char c ;
	
		while (interpretor.next(c))
			interpretor.consume(c);
//		interpretor.consume('\0');

		if(!(interpretor.exact().empty()))
		{
			if (!(interpretor.exact().compare(m_clientPath) == 0))
				continue;
			else
			{
					m_locationBlock = *it;
					break;
			}
		}
		if(!(interpretor.start().empty()))
		{
			if (!(interpretor.start().compare(0, m_clientPath.size(), m_clientPath) == 0))
				continue;
		}
		if(!(interpretor.middle().empty()))
		{
			
		}
		if(!(interpretor.end().empty()))
		{
			std::string end = interpretor.end();
			int size = end.size();
			int index = m_clientPath.size() - size;
			if (end.compare(index, size, m_clientPath) == 0)
			{
				this->location(*it);
				break;
			}
						
		}
		
		it ++;
	}
}

