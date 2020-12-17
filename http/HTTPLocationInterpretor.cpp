/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPLocationInterpretor.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:11:28 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/17 18:08:00 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTPLocationInterpretor.hpp>

HTTPLocationInterpretor::HTTPLocationInterpretor() :
		m_state(S_NOT_STARTED),
		m_path(""),
		m_start(""),
		m_middleList(),
		m_middle(""),
		m_end("")
{
}

HTTPLocationInterpretor::HTTPLocationInterpretor(std::string path) :
		m_state(S_NOT_STARTED),
		m_path(path),
		m_start(""),
		m_middleList(),
		m_middle(""),
		m_end("")
{
}

HTTPLocationInterpretor::HTTPLocationInterpretor(const HTTPLocationInterpretor &other) :
		m_state(other.m_state),
		m_path(other.m_path),
		m_start(other.m_start),
		m_middleList(other.m_middleList),
		m_middle(other.m_middle),
		m_end(other.m_end)
{
}

HTTPLocationInterpretor::~HTTPLocationInterpretor()
{
}

HTTPLocationInterpretor&
HTTPLocationInterpretor::operator =(const HTTPLocationInterpretor &other)
{
	if (this != &other)
	{
		m_state = other.m_state;
		m_path = other.m_path;
		m_start = other.m_start;
		m_middleList = other.m_middleList;
		m_middle = other.m_middle;
		m_end = other.m_end;
	}
	return (*this);
}

bool
HTTPLocationInterpretor::next(char &c)
{
	if (m_path.empty())
		return (false);

	c = m_path[0];

	m_path.erase(0, 1);

	return (true);
}

void
HTTPLocationInterpretor::consume(char &c)
{
	switch (m_state)
	{
		case S_NOT_STARTED:
		case S_FIRST:
		{
			if (c == ' ')
			{
			}
			else if (c == '/')
			{
				m_start += c;
				m_state = S_START; 
			}
			else if (c == '~')
			{
				m_state = S_TILDE;
			}
			else if (c == '=')
			{
				m_state = S_EXACT_INTRO;
				m_exact += c;
			}
			else if (c == '\0')
			{
				m_state = S_OVER;
			}
			else if (c == '*')
				m_state = S_MIDDLE_START;

			break;
		}
		case S_START:
		{
			// if (c == ' ')
			// {
			// 	m_state = S_OVER; //check for exceptions if it can happen here
			// }
			if (c == '*')
				m_state = S_MIDDLE_START;
			else
				m_start += c;
			
			break;
		}
		
		case S_EXACT_INTRO:
		{
			// if (c == ' ')
			// 	;
			if (c == '*')
				throw Exception("No '*' expected");
			else
			{
				m_exact += c;
				m_state = S_EXACT;
			}
			
			break;
		}
		
		case S_EXACT:
		{
			// if (c == ' ')
			// {
			// 	m_state = S_OVER; //check for exceptions if it can happen here
			// }
			if (c == '*')
				throw Exception("No '*' expected");
			else
			{
				m_exact += c;
				m_state = S_EXACT;
			}
			break;
		}
		
		case S_TILDE:
		{
			if (c == ' ')
			{
				m_state = S_SPACE_AFTER_TILDE;
			}
			else if (c == '*')
			{
				m_state = S_NO_CASE_MODIFIER;
			}
			// else if (c == '.')
			// {
			// 	m_state = S_END;
			// 	m_end += c;
			// }
			else if (c == '^')
			{
				m_state = S_START;	
			}
			else if (c == '/')
			{
				m_state = S_START;	
				m_start += c;
			}
			else
			{
				m_state = S_MIDDLE;
				m_middle += c;
			}
			break;
		}
		
		case S_SPACE_AFTER_TILDE:
		{
			if (c == ' ')
			{
				m_state = S_SPACE_AFTER_TILDE;
			}
			// else if (c == '.')
			// {
			// 	m_state = S_END;
			// 	m_end += c;
			// }
			else if (c == '^')
			{
				m_state = S_START;	
			}
			else if (c == '/')
			{
				m_state = S_START;	
				m_start += c;
			}
			else if (c == '*')
				m_state = S_MIDDLE_START;
			else
			{
				m_state = S_MIDDLE;
				m_middle += c;
			}
			break;
		}

		case S_MIDDLE_START :
		{
			if (c == '*')
				m_state = S_MIDDLE_START;
			else
			{
				m_state = S_MIDDLE;
				m_middle += c;
			}	
		}

		case S_MIDDLE :
		{
			if (c == '*')
			{
				m_middleList.push_back(m_middle);
				m_middle = "";
				m_state = S_MIDDLE_START;
			}
			else
			{
				m_state = S_MIDDLE;
				m_middle += c;
			}	
		}
		
		case S_END :
			break;
		case S_OVER : // a voir ou supprimer
			break;
		case S_NO_CASE_MODIFIER :
			break;
		// {
		// 	if (c == ' ')
		// 	{
		// 		m_state = S_OVER; //check for exceptions if it can happen here
		// 	}
		// 	else
		// 	{
		// 		m_end += c;
		// 	}
		// 	break;
		// }
	}
	m_lastChar = c;
}

const std::string&
HTTPLocationInterpretor::start(void)
{
	return (m_start);
}

const std::string&
HTTPLocationInterpretor::end(void)
{
	return (m_end);
}

const std::string&
HTTPLocationInterpretor::exact(void)
{
	return (m_exact);
}

void
HTTPLocationInterpretor::end(std::string &end)
{
	m_end = end;
}

const char&
HTTPLocationInterpretor::lastChar(void)
{
	return (m_lastChar);
}

std::list<std::string>&
HTTPLocationInterpretor::middleList(void)
{
	return (m_middleList);
}

void
HTTPLocationInterpretor::erase(std::list<std::string>::iterator it)
{
	m_middleList.erase(it);
}

void
HTTPLocationInterpretor::setEnd(std::string end)
{
	m_end = end;
}
