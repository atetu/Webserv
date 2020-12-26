/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestProcessor.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 15:59:39 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/26 15:59:39 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <exception/Exception.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/enums/HTTPVersion.hpp>
#include <http/handler/HTTPMethodHandler.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/request/HTTPRequestProcessor.hpp>
#include <http/request/parser/HTTPRequestParser.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <http/response/impl/cgi/CGIHTTPResponse.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <http/route/HTTPFindLocation.hpp>
#include <util/buffer/impl/BaseBuffer.hpp>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <util/Enum.hpp>
#include <util/Environment.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/Optional.hpp>
#include <util/URL.hpp>
#include <iostream>
#include <list>
#include <map>
#include <string>

Logger &HTTPRequestProcessor::LOG = LoggerFactory::get("HTTP Request Processor");

HTTPRequestProcessor::HTTPRequestProcessor(const Configuration &configuration, const Environment &environment) :
		m_configuration(configuration),
		m_environment(environment)
{
}

HTTPRequestProcessor::~HTTPRequestProcessor()
{
}

void
HTTPRequestProcessor::process(HTTPClient &client)
{
	const ServerBlock *serverBlockPtr = findServerBlock(client);
	if (!serverBlockPtr)
	{
		client.response() = GenericHTTPResponse::status(*HTTPStatus::NOT_FOUND);
		return;
	}

	const HTTPMethod *methodPtr = HTTPMethod::find(client.parser().method());
	if (!methodPtr)
	{
		client.response() = GenericHTTPResponse::status(*HTTPStatus::METHOD_NOT_ALLOWED);
		return;
	}

	const ServerBlock &serverBlock = *serverBlockPtr;
	const HTTPMethod &method = *methodPtr;

	if (serverBlock.methods().present() && !serverBlock.hasMethod(method.name()))
	{
		client.response() = GenericHTTPResponse::status(*HTTPStatus::METHOD_NOT_ALLOWED);
		return;
	}

	const LocationBlock *locationBlockPtr = NULL;
	if (serverBlockPtr && serverBlockPtr->locations().present())
	{
		HTTPFindLocation findLocation(client.parser().path(), serverBlockPtr->locations().get());

		if (findLocation.parse().location().present())
			locationBlockPtr = findLocation.parse().location().get();
	}

	if (locationBlockPtr)
	{
		const LocationBlock &locationBlock = *locationBlockPtr;

		if (locationBlock.methods().present() && !locationBlock.hasMethod(method.name()))
		{
			client.response() = GenericHTTPResponse::status(*HTTPStatus::METHOD_NOT_ALLOWED);
			return;
		}
	}

	if (method.hasBody())
		client.parser().body(client.in().storage());

	const Optional<std::map<std::string, std::string> > queryMap = Optional<std::map<std::string, std::string> >::ofEmpty(client.parser().query());
	const Optional<std::string> fragmentStr = Optional<std::string>::ofEmpty(client.parser().fragment());

	URL url = URL("http", "locahost", client.server().port(), client.parser().path(), queryMap, fragmentStr); // TODO Move to parser::getUrl()

	const RootBlock &rootBlock = m_configuration.rootBlock();

	const HTTPVersion &version = HTTPVersion::HTTP_1_1;
	const HTTPHeaderFields &headerFields = client.parser().headerFields();
	const Optional<LocationBlock const*> locationBlockOptional = Optional<LocationBlock const*>::ofNullable(locationBlockPtr);

	const std::string body = client.parser().body();

	client.request() = new HTTPRequest(method, url, version, headerFields, body, m_configuration, rootBlock, *serverBlockPtr, locationBlockOptional);

	if (locationBlockPtr)
	{
		const LocationBlock &locationBlock = *locationBlockPtr;

		if (locationBlock.methods().present())
		{
			if (!locationBlock.hasMethod(method.name()))
			{
				client.response() = GenericHTTPResponse::status(*HTTPStatus::METHOD_NOT_ALLOWED);
				return;
			}
		}

		if (!client.response() && locationBlock.cgi().present())
		{
			const CGIBlock &cgiBlock = rootBlock.getCGI(locationBlockPtr->cgi().get());

			try
			{
				CommonGatewayInterface *cgi = CommonGatewayInterface::execute(client, cgiBlock, m_environment);

				client.response() = new CGIHTTPResponse(HTTPStatusLine(*HTTPStatus::OK), *cgi);
			}
			catch (Exception &exception)
			{
				LOG.debug() << "An error occurred while processing CGI: " << exception.message() << std::endl;

				client.response() = GenericHTTPResponse::status(*HTTPStatus::BAD_GATEWAY);
			}
		}
	}

	if (!client.response())
		client.response() = method.handler().handle(*client.request());
}

const ServerBlock*
HTTPRequestProcessor::findServerBlock(HTTPClient &client)
{
	typedef std::list<ServerBlock const*> list;
	typedef list::const_iterator iterator;

	const list &serverBlocks = client.server().serverBlocks();

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

	return (client.server().defaultServerBlock());
}
