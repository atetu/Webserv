/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 13:18:11 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 17:03:13 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <http/request/HTTPRequest.hpp>
#include <io/File.hpp>

class HTTPVersion;

HTTPRequest::~HTTPRequest(void)
{
}

std::string
HTTPRequest::root(void) const
{
	if (location().present())
	{
		const LocationBlock &locationBlock = *location().get();

		if (locationBlock.root().present())
			return (locationBlock.root().get());
	}

	if (server().root().present())
		return (server().root().get());

	if (configuration().rootBlock().root().present())
		return (configuration().rootBlock().root().get());

	return (File::currentDirectory().path());
}

bool
HTTPRequest::needAuth() const
{
	return (server().auth().present() || (location().present() && location().get()->auth().present()));
}

const AuthBlock*
HTTPRequest::auth() const
{
	if (location().present() && location().get()->auth().present())
		return (location().get()->auth().get());

	return (server().auth().get());
}
