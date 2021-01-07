/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFilter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 19:03:57 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 19:03:57 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/ServerBlock.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/before/ServerFilter.hpp>
#include <http/filter/x/Request.hpp>
#include <http/filter/x/Response.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <util/Optional.hpp>
#include <list>
#include <string>

ServerFilter::ServerFilter()
{
}

ServerFilter::~ServerFilter()
{
}

ServerFilter::ServerFilter(const ServerFilter &other)
{
	(void)other;
}

ServerFilter&
ServerFilter::operator=(const ServerFilter &other)
{
	(void)other;

	return (*this);
}

void
ServerFilter::doFilter(HTTPClient &client, Request &request, Response &response, FilterChain &next)
{
	const ServerBlock *serverBlockPtr = findBlock(client);
	if (serverBlockPtr)
	{
		request.serverBlock(*serverBlockPtr);

		return (next());
	}

	response.status(*HTTPStatus::NOT_FOUND);
}

const ServerBlock*
ServerFilter::findBlock(HTTPClient &client)
{
	typedef std::list<ServerBlock const*> list;
	typedef list::const_iterator iterator;

	const list &serverBlocks = client.httpServer().serverBlocks();

	Optional<std::string> hostOptional = client.parser().headerFields().get(HTTPHeaderFields::HOST);

	if (hostOptional.present())
	{
		const std::string &host = hostOptional.get();

		for (iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
		{
			const ServerBlock &serverBlock = *(*it);

			if (serverBlock.hasName(host))
				return (&serverBlock);
		}
	}

	return (client.httpServer().defaultServerBlock());
}
