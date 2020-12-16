/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPFindLocation.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 15:24:50 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/16 17:07:53 by alicetetu        ###   ########.fr       */
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
		int start;
		int end;
	
		while (interpretor.next(c))
			interpretor.consume(c);

		if(!(interpretor.exact().empty()))
		{
			std::cout << "exact\n";
			if (!(interpretor.exact().compare(m_clientPath) == 0))
			{
				it++;
				continue;
			}
			else
			{
				m_locationBlock = *it;
				break;
			}
		}
		if(!(interpretor.start().empty()))
		{
			if (!(m_clientPath.compare(0, interpretor.start().size(), interpretor.start()) == 0))
			{	
				it++;
				continue;
			}
			start = 1;
		}
		
		if(!(interpretor.end().empty()))
		{
			
			std::string end = interpretor.end();
			
			int size = end.size();
			int index = m_clientPath.size() - size;
			if (m_clientPath.compare(index, size, end) == 0)
				end = 1;
			else
			{	
				it++;
				continue;
			}
						
		}
		if (start || end)
		{
			this->location(*it);
			break;
		}	
		
		it++;
	}
}

