/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:51:19 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 18:51:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/enums/HTTPStatus.hpp>
#include <http/filter/x/Response.hpp>

Response::Response() :
		m_status(),
		m_headers(),
		m_body()
{
}

Response::Response(const Response &other) :
		m_status(other.m_status),
		m_headers(other.m_headers),
		m_body(other.m_body)
{
}

Response::~Response()
{
}

Response&
Response::operator=(const Response &other)
{
	if (this != &other)
	{
		m_status = other.m_status;
		m_headers = other.m_headers;
		m_body = other.m_body;
	}

	return (*this);
}

void
Response::status(const HTTPStatus &status)
{
	m_status.set(&status);
}

Optional<const HTTPStatus*>&
Response::status()
{
	return (m_status);
}

HTTPHeaderFields&
Response::headers()
{
	return (m_headers);
}
void
Response::body(IBody *body, bool deletePrevious)
{
	if (deletePrevious && m_body)
		delete m_body;

	m_body = body;
}

Response::IBody*
Response::body()
{
	return (m_body);
}

Response::IBody::~IBody()
{
}

Response::StringBody::~StringBody()
{
}

Response::FileBody::~FileBody()
{
}

Response::CGIBody::~CGIBody()
{
}

Response::StringBody::StringBody(const std::string &content)
{
	(void)content;
}

Response::FileBody::FileBody(FileDescriptor &fd)
{
	fd.close();
	delete &fd;
}
