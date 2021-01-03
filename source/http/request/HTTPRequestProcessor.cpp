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

#include <buffer/impl/BaseBuffer.hpp>
#include <buffer/impl/SocketBuffer.hpp>
#include <config/block/AuthBlock.hpp>
#include <config/block/CGIBlock.hpp>
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
#include <http/parser/HTTPRequestParser.hpp>
#include <http/parser/HTTPRequestPathParser.hpp>
#include <http/request/HTTPRequestProcessor.hpp>
#include <http/request/impl/GenericHTTPRequest.hpp>
#include <http/request/impl/SubHTTPRequest.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <http/response/impl/cgi/CGIHTTPResponse.hpp>
#include <http/route/HTTPFindLocation.hpp>
#include <io/File.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <stddef.h>
#include <util/Enum.hpp>
#include <util/Environment.hpp>
#include <util/Optional.hpp>
#include <util/StringUtils.hpp>
#include <util/URL.hpp>
#include <iostream>
#include <list>
#include <string>

Logger &HTTPRequestProcessor::LOG = LoggerFactory::get("Req. Processor");

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
		client.response() = HTTPMethodHandler::error(*client.request(), *HTTPStatus::NOT_FOUND);
		return;
	}

	const HTTPMethod *methodPtr = HTTPMethod::find(client.parser().method());
	if (!methodPtr)
	{
		client.response() = HTTPMethodHandler::error(*client.request(), *HTTPStatus::METHOD_NOT_ALLOWED);
		return;
	}

	const ServerBlock &serverBlock = *serverBlockPtr;
	const HTTPMethod &method = *methodPtr;

	if (serverBlock.methods().present() && !serverBlock.hasMethod(method.name()))
	{
		client.response() = HTTPMethodHandler::error(*client.request(), *HTTPStatus::METHOD_NOT_ALLOWED);
		return;
	}

	const LocationBlock *locationBlockPtr = NULL;
	if (serverBlockPtr && serverBlockPtr->locations().present())
	{
		HTTPFindLocation findLocation(client.parser().pathParser().path(), serverBlockPtr->locations().get());

		if (findLocation.parse().location().present())
			locationBlockPtr = findLocation.parse().location().get();
	}

	if (locationBlockPtr)
	{
		const LocationBlock &locationBlock = *locationBlockPtr;

		if (locationBlock.methods().present() && !locationBlock.hasMethod(method.name()))
		{
			client.response() = HTTPMethodHandler::error(*client.request(), *HTTPStatus::METHOD_NOT_ALLOWED);
			return;
		}
	}

	if (method.hasBody())
		client.parser().body(client.in().storage(), serverBlock.maxBodySize());

	const HTTPVersion &version = HTTPVersion::HTTP_1_1;
	const HTTPHeaderFields &headerFields = client.parser().headerFields();
	const Optional<LocationBlock const*> locationBlockOptional = Optional<LocationBlock const*>::ofNullable(locationBlockPtr);

	const std::string &body = client.parser().body();

	URL url = client.parser().url(); // TODO Need fix

	client.request() = new GenericHTTPRequest(method, url, version, headerFields, body, m_configuration, *serverBlockPtr, locationBlockOptional);

	if (client.request()->needAuth()) // TODO Need to be moved
	{
		const AuthBlock *authBlock = client.request()->auth();

		Optional<std::string> authorizationOptional = client.request()->headers().get(HTTPHeaderFields::AUTHORIZATION);

		bool authorized = false;

		if (authorizationOptional.present())
		{
			const std::string &authorization = authorizationOptional.get();

			size_t pos = authorization.find(' ');
			if (pos != std::string::npos)
			{
				std::string type = authorization.substr(0, pos);
				std::string credentials = authorization.substr(pos + 1);

				if (StringUtils::equalsIgnoreCase(authBlock->type(), type) && authBlock->authorize(credentials))
					authorized = true;
			}
		}

		if (!authorized)
		{
			client.response() = HTTPMethodHandler::error(*client.request(), *HTTPStatus::UNAUTHORIZED, HTTPHeaderFields().wwwAuthenticate(authBlock->prettyType(), authBlock->realm()));
			return;
		}
	}

	if (locationBlockPtr)
	{
		const LocationBlock &locationBlock = *locationBlockPtr;

		if (locationBlock.methods().present())
		{
			if (!locationBlock.hasMethod(method.name()))
			{
				client.response() = HTTPMethodHandler::error(*client.request(), *HTTPStatus::METHOD_NOT_ALLOWED);
				return;
			}
		}
	}

	if (method == *HTTPMethod::GET || method == *HTTPMethod::HEAD)
	{
		File targetFile(client.request()->root(),client.request()->url().path());

		if (targetFile.exists() && targetFile.isDirectory())
		{
			if (StringUtils::last(client.request()->url().path()) != '/')
			{
				client.response() = HTTPMethodHandler::redirect(*HTTPStatus::MOVED_PERMANENTLY, client.request()->url().builder().appendToPath("/").build());
				return;
			}

			if (locationBlockPtr)
			{
				const LocationBlock &locationBlock = *locationBlockPtr;

				if (locationBlock.index().present())
				{
					const std::list<std::string> &indexFiles = locationBlock.index().get();

					for (std::list<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); it++)
					{
						File anIndex(targetFile, *it);

						if (anIndex.exists() && anIndex.isFile())
						{
							File newPath(client.request()->url().path(), *it);

							client.request() = new SubHTTPRequest(*static_cast<GenericHTTPRequest*>(client.request()), method, newPath.path());
							break;
						}
					}
				}
			}
		}
	}

	if (locationBlockPtr)
	{
		const LocationBlock &locationBlock = *locationBlockPtr;
		if (!client.response() && locationBlock.cgi().present())
		{
			const CGIBlock &cgiBlock = m_configuration.rootBlock().getCGI(locationBlock.cgi().get());

			std::string extension;
			if (File::findExtension(client.request()->resource(), extension))
			{
				if (cgiBlock.hasExtension(extension))
				{
					try
					{
						CommonGatewayInterface *cgi = CommonGatewayInterface::execute(client, cgiBlock, m_environment);

						client.response() = new CGIHTTPResponse(HTTPStatusLine(*HTTPStatus::OK), *cgi);
					}
					catch (Exception &exception)
					{
						LOG.debug() << "An error occurred while processing CGI: " << exception.message() << std::endl;

						client.response() = HTTPMethodHandler::error(*client.request(), *HTTPStatus::BAD_GATEWAY);
					}
				}
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
