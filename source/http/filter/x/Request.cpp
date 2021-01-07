/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:51:13 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 18:51:13 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/RootBlock.hpp>
#include <config/Configuration.hpp>
#include <http/filter/x/Request.hpp>
#include <io/File.hpp>
#include <util/Collections.hpp>
#include <util/Enum.hpp>
#include <map>

Request::Request() :
		m_configuration(NULL),
		m_url(),
		m_method(),
		m_serverBlock(),
		m_locationBlock(),
		m_headers(),
		m_resource()
{
}

Request::Request(const Configuration &configuration, URL url) :
		m_configuration(&configuration),
		m_url(url),
		m_method(),
		m_serverBlock(),
		m_locationBlock(),
		m_headers(),
		m_resource(url.path())
{
}

Request::Request(const Request &other) :
		m_configuration(other.m_configuration),
		m_url(other.m_url),
		m_method(other.m_method),
		m_serverBlock(other.m_serverBlock),
		m_locationBlock(other.m_locationBlock),
		m_headers(other.m_headers),
		m_resource(other.m_resource)
{
}

Request::~Request()
{
}

Request&
Request::operator=(const Request &other)
{
	if (this != &other)
	{
		m_configuration = other.m_configuration;
		m_url = other.m_url;
		m_method = other.m_method;
		m_serverBlock = other.m_serverBlock;
		m_locationBlock = other.m_locationBlock;
		m_headers = other.m_headers;
		m_resource = other.m_resource;
	}

	return (*this);
}

const URL&
Request::url(void) const
{
	return (m_url);
}

void
Request::method(const HTTPMethod &method)
{
	m_method = &method;
}

Optional<const HTTPMethod*>
Request::method(void) const
{
	return (Optional<const HTTPMethod*>::ofNullable(m_method));
}

void
Request::serverBlock(const ServerBlock &serverBlock)
{
	m_serverBlock = &serverBlock;
}

Optional<const ServerBlock*>
Request::serverBlock(void) const
{
	return (Optional<const ServerBlock*>::ofNullable(m_serverBlock));
}

void
Request::locationBlock(const LocationBlock &locationBlock)
{
	m_locationBlock = &locationBlock;
}

const Optional<const LocationBlock*>
Request::locationBlock(void) const
{
	return (Optional<const LocationBlock*>::ofNullable(m_locationBlock));
}

HTTPHeaderFields&
Request::headers()
{
	return (m_headers);
}

const std::string&
Request::resource() const
{
	return (m_resource);
}

void
Request::resource(const std::string &resource)
{
	m_resource = resource;
}

std::list<const HTTPMethod*>
Request::allowedMethods()
{
	std::map<int, const HTTPMethod*> allowedMap;

	if (m_locationBlock && m_locationBlock->methods().present())
	{
		typedef std::list<std::string> list;

		const list &methods = m_locationBlock->methods().get();
		for (list::const_iterator it = methods.begin(); it != methods.end(); it++)
		{
			const HTTPMethod *methodPtr = HTTPMethod::find(*it);

			if (methodPtr)
				allowedMap[methodPtr->ordinal()] = methodPtr;
		}
	}

	if (m_serverBlock && m_serverBlock->methods().present())
	{
		typedef std::list<std::string> list;

		const list &methods = m_serverBlock->methods().get();
		for (list::const_iterator it = methods.begin(); it != methods.end(); it++)
		{
			const HTTPMethod *methodPtr = HTTPMethod::find(*it);

			if (methodPtr)
				allowedMap[methodPtr->ordinal()] = methodPtr;
		}
	}

	return (Collections::values<int, const HTTPMethod*>(allowedMap));
}

Optional<const AuthBlock*>
Request::authBlock() const
{
	if (m_locationBlock && m_locationBlock->auth().present())
		return (m_locationBlock->auth().get());

//	if (m_serverBlock && m_serverBlock->auth().present())
//		return (m_serverBlock->auth().get());

	return (Optional<const AuthBlock*>());
}

File
Request::targetFile()
{
	return (File(root(), m_resource));
}

std::string
Request::root(void) const
{
	if (locationBlock().present())
	{
		const LocationBlock &locationBlock = *this->locationBlock().get();

		if (locationBlock.root().present())
			return (locationBlock.root().get());
	}

	if (serverBlock().present())
	{
		const ServerBlock &serverBlock = *this->serverBlock().get();

		if (serverBlock.root().present())
			return (serverBlock.root().get());
	}

	if (m_configuration && m_configuration->rootBlock().root().present())
		return (m_configuration->rootBlock().root().get());

	return (File::currentDirectory().path());
}
