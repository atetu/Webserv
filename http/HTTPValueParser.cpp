/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPValueParser.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 16:09:31 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/14 14:11:14 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <exception/Exception.hpp>
#include <http/HTTPValueParser.hpp>
#include <cctype>
#include <string>

HTTPValueParser::HTTPValueParser() :
		m_state(S_NOT_STARTED),
		m_globalValue(""),
		m_weightNb(1),
		m_map(new std::map<float, std::list<std::string> >)
{
	m_valueElement.reserve(16);
	m_weightStr.reserve(5); // ou 6???
}

HTTPValueParser::HTTPValueParser(std::string globalValue) :
		m_state(S_NOT_STARTED),
		m_globalValue(globalValue),
		m_weightNb(1),
		m_map(new std::map<float, std::list<std::string> >)
{
	m_valueElement.reserve(16);
	m_weightStr.reserve(5); // ou 6???
}

HTTPValueParser::~HTTPValueParser()
{
}

void
HTTPValueParser::consume(char c)
{
	switch (m_state)
	{
		case S_NOT_STARTED:
		case S_VALUE:
		{
			if (c == ' ')
			{
				if (m_state != S_NOT_STARTED)
					m_state = S_SPACES_AFTER_VALUE;
			}
			else if (c == ',')
			{
				(*m_map)[m_weightNb].push_back(m_valueElement);
				m_valueElement = "";
				m_valueElement.reserve(16);
				m_state = S_NOT_STARTED; 
			}
			else if (c == ';')
			{
				m_state = S_SEMI_COLON;
			}
			else if (c == '\0')
			{
				(*m_map)[m_weightNb].push_back(m_valueElement);
				m_weightNb = 1;
				m_weightStr = "";
				m_weightStr.reserve(5); // ou 6
				
				m_state = S_END;
			}
			else
			{
				m_state = S_VALUE;
				m_valueElement += c;
			}

			break;
		}
		case S_SPACES_AFTER_VALUE:
		{
			if (c == ' ')
				;
			else if (c == ',')
			{
				(*m_map)[m_weightNb].push_back(m_valueElement);
				m_valueElement = "";
				m_valueElement.reserve(16);
				m_state = S_NOT_STARTED; 
			}
			else if (c == ';')
			{
				m_state = S_SEMI_COLON;
			}
			else
			{
				m_state = S_VALUE;
				m_valueElement += c;
			}

			break;
		}
		
		case S_SEMI_COLON:
		{
			if (c == ' ')
				;
			else if (c == 'q')
			{
				m_state = S_WEIGHT_START1; 
			}
			else 
				throw Exception("No q parameter");
			break;
		}
		
		case S_WEIGHT_START1:
		{
			if (c == '=')
				m_state = S_WEIGHT_START2;
			else 
				throw Exception("Missing '=' after q");
			break;
		}
		
		case S_WEIGHT_START2:
		{
			if (c == ' ')
				m_state = S_SPACES_BEFORE_WEIGHT;
			else
			{
				m_state = S_WEIGHT;
				m_weightStr += c;
			}
			
			break;
		}
		

		case S_SPACES_BEFORE_WEIGHT:
		{
			if (c == ' ')
				m_state = S_SPACES_BEFORE_WEIGHT;
			else
			{
				m_state = S_WEIGHT;
				m_weightStr += c;
			}
			
			break;
		}
		
		case S_WEIGHT:
		{
			if (c == ' ')
			{
				m_weightNb = std::stof(m_weightStr);
				(*m_map)[m_weightNb].push_back(m_valueElement);
				m_valueElement = "";
				m_valueElement.reserve(16);
				m_weightNb = 1;
				m_weightStr = "";
				m_weightStr.reserve(5); // ou 6
				
				m_state = S_SPACES_AFTER_WEIGHT;
			}
			else if (c == '\0')
			{
				m_weightNb = std::stof(m_weightStr);
				(*m_map)[m_weightNb].push_back(m_valueElement); 
				
				m_valueElement = "";
				m_valueElement.reserve(16);
				m_weightNb = 1;
				m_weightStr = "";
				m_weightStr.reserve(5); // ou 6
				
				m_state = S_END;
			}
			else if (c == ',')
			{
				m_weightNb = std::stof(m_weightStr);
				(*m_map)[m_weightNb].push_back(m_valueElement);
				m_valueElement = "";
				m_valueElement.reserve(16);
				m_weightNb = 1;
				m_weightStr = "";
				m_weightStr.reserve(5); // ou 6
				
				m_state = S_NOT_STARTED;
			}
			else
			{
				m_state = S_WEIGHT;
				m_weightStr += c;
			}
			
			break;
		}
		
		case S_SPACES_AFTER_WEIGHT:
		{
			if (c == ' ')
			{
				m_state = S_SPACES_AFTER_WEIGHT;
			}
			else if (c == ',')
			{
				m_state = S_NOT_STARTED;
			}
			else
				throw Exception("Space, \r, or comma expected after weight");
			
			break;
		}
		
		case S_END:
		{
			break;
		}
				
	}
}

HTTPValueParser::State
HTTPValueParser::state() const
{
	return (m_state);
}

std::map<float, std::list<std::string> >*
HTTPValueParser::getMap() const
{
	return (m_map);
}

bool
HTTPValueParser::next(char &c)
{
	if (m_globalValue.empty())
		return (false);

	c = m_globalValue[0];

	m_globalValue.erase(0, 1);

	return (true);
}