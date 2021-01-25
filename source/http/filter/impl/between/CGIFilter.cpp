/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIFilter.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 00:27:23 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/25 19:31:07 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/CGIBlock.hpp>
#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/Configuration.hpp>
#include <exception/Exception.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/cgi/task/CGITask.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/filter/impl/between/CGIFilter.hpp>
#include <http/HTTPClient.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/HTTPResponse.hpp>
#include <io/File.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <util/Environment.hpp>
#include <util/FilenameUtils.hpp>
#include <util/Macros.hpp>
#include <util/Optional.hpp>
#include <iostream>
#include <string>

Logger &CGIFilter::LOG = LoggerFactory::get("CGI Filter");

CGIFilter::CGIFilter()
{
}

CGIFilter::CGIFilter(const CGIFilter &other)
{
	(void)other;
}

CGIFilter::~CGIFilter()
{
}

CGIFilter&
CGIFilter::operator=(const CGIFilter &other)
{
	(void)other;

	return (*this);
}

void
CGIFilter::doFilter(UNUSED HTTPClient &client, UNUSED HTTPRequest &request, UNUSED HTTPResponse &response, FilterChain &next)
{
	std::cout << "CGI\n";
	if (request.locationBlock().absent())
		return (next());
	std::cout << "CGI2\n";
	const LocationBlock &locationBlock = *request.locationBlock().get();
	if (locationBlock.cgi().absent())
		return (next());
	std::cout << "CGI3\n";
	std::string extension;
	if ((extension = FilenameUtils::getExtension(request.resource())).empty())
		return (next());
std::cout << "CGI4\n" << extension << std::endl;
std::cout << "---------" << std::endl;

	const CGIBlock &cgiBlock = Configuration::instance().rootBlock().getCGI(locationBlock.cgi().get());
std::cout << cgiBlock.extensions().get().size() << std::endl;
std::cout << Convert::join(cgiBlock.extensions().get()) << std::endl;
std::cout << "---------" << std::endl;
	if (!cgiBlock.hasExtension(extension))
		return (next());
std::cout << "CGI5\n";
	if (!cgiBlock.handleNotFound().orElse(false) && !request.targetFile().exists())
	{
		std::cout << "CGI5 - not found: " << request.targetFile().path() << std::endl;
		client.response().status(*HTTPStatus::NOT_FOUND);
		return (next());
	}

	std::cout << "CGI6\n";
	File cgiFile(cgiBlock.path().get());
	std::cout << cgiBlock.path().get() << std::endl;
	if (!cgiFile.exists() || !cgiFile.isFile() || !cgiFile.isExecutable())
	{
		client.response().status(*HTTPStatus::BAD_GATEWAY);
		return (next());
	}

	try
	{
		client.task(*CommonGatewayInterface::execute(client, cgiBlock, Environment::get()));
		client.response().status(*HTTPStatus::OK);
	}
	catch (Exception &exception)
	{
		LOG.debug() << "An error occurred while executing CGI: " << exception.message() << std::endl;

		client.response().status(*HTTPStatus::BAD_GATEWAY);
		return (next());
	}
}
