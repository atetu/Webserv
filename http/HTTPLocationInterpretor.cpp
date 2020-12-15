/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPLocationInterpretor.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:11:28 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/15 17:29:59 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <http/HTTPLocationInterpretor.hpp>

HTTPLocationInterpretor::HTTPLocationInterpretor() :
		m_state(S_NOT_STARTED),
		m_path(""),
		m_start(""),
		m_middle(""),
		m_end("")
{
}

HTTPLocationInterpretor::HTTPLocationInterpretor(std::string path) :
		m_state(S_NOT_STARTED),
		m_path(path),
		m_start(""),
		m_middle(""),
		m_end("")
{
}

HTTPLocationInterpretor::HTTPLocationInterpretor(const HTTPLocationInterpretor &other) :
		m_state(other.m_state),
		m_path(other.m_path),
		m_start(other.m_start),
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

void
HTTPLocationInterpretor::consume(char &c)
{
	switch (m_state)
	{
		case S_NOT_STARTED:
		case S_START:
		{
			if (c == ' ')
			{
			}
			else if (c == '/')
			{
				(*m_map)[m_weightNb].push_back(m_valueElement);
				m_valueElement = "";
				m_valueElement.reserve(16);
				m_state = S_STARTED; 
			}
			else if (c == '.')
			{
				m_state = S_END;
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
}