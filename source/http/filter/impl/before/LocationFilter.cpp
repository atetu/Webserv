/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationFilter.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 19:10:43 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/25 19:23:57 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/before/LocationFilter.hpp>
#include <http/HTTPClient.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <http/parser/HTTPRequestPathParser.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/route/HTTPFindLocation.hpp>
#include <log/LoggerFactory.hpp>
#include <util/Macros.hpp>
#include <util/Optional.hpp>
#include <list>
#include <string>

Logger &LocationFilter::LOG = LoggerFactory::get("Location Filter");

LocationFilter::LocationFilter()
{
}

LocationFilter::LocationFilter(const LocationFilter &other)
{
	(void)other;
}

LocationFilter::~LocationFilter()
{
}

LocationFilter&
LocationFilter::operator=(const LocationFilter &other)
{
	(void)other;

	return (*this);
}

void
LocationFilter::doFilter(UNUSED HTTPClient &client, HTTPRequest &request, UNUSED HTTPResponse &response, FilterChain &next)
{
	const ServerBlock &serverBlock = *request.serverBlock().get(); /* Should always have one. */
	const Optional<std::list<const LocationBlock*> > &locations = serverBlock.locations();

	if (locations.absent())
		return (next());

	const std::string &path = client.parser().pathParser().path();

	HTTPFindLocation findLocation(path, locations.get());
	if (findLocation.parse().location().present())
	{
		const LocationBlock &locationBlock = *findLocation.parse().location().get();

		request.locationBlock(locationBlock);

			if (locationBlock.root().present() /* && StringUtils::last(locationBlock.path()) != '/'*/) // TODO see comment
			{
				std::string path;
				if (locationBlock.path().c_str()[locationBlock.path().size() -1] == '/')
				{
					if (locationBlock.path().size() <= request.resource().size())
						path = request.resource().substr(locationBlock.path().size(), std::string::npos);
					else
					{
						path = "";
					}
					
				}
				else
				{
					path = locationBlock.path();
				}
				std::cout << path<< std::endl;
				request.resource(path);
			}
		}
	return (next());
}
