/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/03 11:46:08 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/CGIBlock.hpp>
#include <config/block/LocationBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <util/json/JsonBoolean.hpp>
#include <util/json/JsonNumber.hpp>
#include <util/json/JsonString.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/unit/DataSize.hpp>
#include <iostream>

Logger &Configuration::LOG = LoggerFactory::get("Configuration");

Configuration::Configuration() :
		m_file(),
		m_mimeRegistry(NULL),
		m_rootBlock(NULL)
{
}

Configuration::Configuration(const std::string &file, const MimeRegistry &mimeRegistry, const RootBlock &rootBlock) :
		m_file(file),
		m_mimeRegistry(&mimeRegistry),
		m_rootBlock(&rootBlock)
{
}

Configuration::Configuration(const Configuration &other) :
		m_file(other.m_file),
		m_mimeRegistry(other.m_mimeRegistry),
		m_rootBlock(other.m_rootBlock)
{
}

Configuration::~Configuration()
{
}

Configuration&
Configuration::operator =(const Configuration &other)
{
	if (this != &other)
	{
		m_file = other.m_file;
		m_mimeRegistry = other.m_mimeRegistry;
		m_rootBlock = other.m_rootBlock;
	}

	return (*this);
}

Configuration&
Configuration::build()
{
	return (*this);
}

#define BIND(object, key, jsonType, type, to, apply) \
			BIND_CUSTOM(object, key, jsonType, type, to->apply((type) value));

#define BIND_CUSTOM(object, key, jsonType, type, apply)                                 \
			if ((object).has(key))                                                      \
			{                                                                           \
				std::string ipath = path + KEY_DOT + key;                               \
				const JsonValue *jsonValue = (object).get(key);                         \
																						\
				type value = jsonCast<jsonType>(ipath, jsonValue);                      \
																						\
				do {                                                                    \
					apply;                                                              \
				} while (0);                                                            \
			}

Configuration
Configuration::fromJsonFile(const std::string &filepath)
{
	JsonObject *jsonObject = JsonReader::fromFile(filepath).readObject();

	RootBlock *rootBlock = FromJsonBuilder::buildRootBlock(*jsonObject);
	MimeRegistry *mimeRegistry = new MimeRegistry();

	LOG.trace() << "Root Block: " << rootBlock << std::endl;

	return (Configuration(filepath, *mimeRegistry, *rootBlock));
}

JsonObject&
Configuration::FromJsonBuilder::rootObject(JsonReader &jsonReader)
{
	JsonValue *jsonValue = jsonReader.read();

	try
	{
		if (!jsonValue->instanceOf<JsonObject>())
			throw Exception("Root JSON Value must be an Object, got: " + jsonValue->typeString());
	}
	catch (...)
	{
		delete jsonValue;

		throw;
	}

	return (*(jsonValue->cast<JsonObject>()));
}

RootBlock*
Configuration::FromJsonBuilder::buildRootBlock(const JsonObject &jsonObject)
{
	RootBlock *rootBlock = new RootBlock();

	try
	{
		if (jsonObject.has(KEY_ROOT_ROOT))
		{
			std::string path = KEY_ROOT KEY_DOT KEY_ROOT_ROOT;
			const JsonString &string = jsonCast<JsonString>(path, jsonObject.get(KEY_ROOT_ROOT));

			rootBlock->root(string);
		}

//		if (jsonObject.has(KEY_ROOT_MIME))
//		{
//			std::string path = KEY_ROOT KEY_DOT KEY_ROOT_MIME;
//			const JsonObject &object = jsonCast<JsonObject>(path, jsonObject.get(KEY_ROOT_MIME));
//
////			rootBlock->mimeBlock(*buildMimeBlock(path, object));
//		}

		if (jsonObject.has(KEY_ROOT_CGI))
		{
			std::string path = KEY_ROOT KEY_DOT KEY_ROOT_CGI;
			const JsonObject &object = jsonCast<JsonObject>(path, jsonObject.get(KEY_ROOT_CGI));

			rootBlock->cgiBlocks(buildBlocks<CGIBlock>(path, object, &Configuration::FromJsonBuilder::buildCGIBlock));
		}

		if (jsonObject.has(KEY_ROOT_SERVERS))
		{
			std::string path = KEY_ROOT KEY_DOT KEY_ROOT_SERVERS;
			const JsonArray &array = jsonCast<JsonArray>(path, jsonObject.get(KEY_ROOT_SERVERS));

			rootBlock->serverBlocks(buildBlocks<ServerBlock>(path, array, &Configuration::FromJsonBuilder::buildServerBlock));
		}
	}
	catch (...)
	{
		delete rootBlock;

		throw;
	}

	return (rootBlock);
}

CGIBlock*
Configuration::FromJsonBuilder::buildCGIBlock(const std::string &path, const std::string &key, const JsonObject &jsonObject)
{
	return (new CGIBlock());
}

ServerBlock*
Configuration::FromJsonBuilder::buildServerBlock(const std::string &path, const JsonObject &jsonObject)
{
	ServerBlock *serverBlock = new ServerBlock();

	try
	{
		BIND(jsonObject, KEY_SERVER_PORT, JsonNumber, int, serverBlock, port);
		BIND(jsonObject, KEY_SERVER_HOST, JsonString, std::string, serverBlock, host);
		BIND(jsonObject, KEY_SERVER_ROOT, JsonString, std::string, serverBlock, root);

		BIND_CUSTOM(jsonObject, KEY_SERVER_MAXBODYSIZE, JsonString, std::string,
		{
			try
			{
				serverBlock->maxBodySize(DataSize::parse(value));
			}
			catch (Exception &exception)
			{
				throw Exception(exception.message() + std::string(" (") + path + ")");
			}
		});

		if (jsonObject.has(KEY_SERVER_LOCATIONS))
		{
			std::string path = KEY_ROOT KEY_DOT KEY_SERVER_LOCATIONS;
			const JsonObject &object = jsonCast<JsonObject>(path, jsonObject.get(KEY_SERVER_LOCATIONS));

			serverBlock->locations(buildBlocks<LocationBlock>(path, object, &Configuration::FromJsonBuilder::buildLocationBlock));
		}
	}
	catch (...)
	{
		delete serverBlock;

		throw;
	}

	return (serverBlock);
}

LocationBlock*
Configuration::FromJsonBuilder::buildLocationBlock(const std::string &path, const std::string &key, const JsonObject &jsonObject)
{
	LocationBlock *locationBlock = new LocationBlock(key);

	try
	{
		BIND(jsonObject, KEY_LOCATION_ALIAS, JsonString, std::string, locationBlock, alias);
		BIND(jsonObject, KEY_LOCATION_ROOT, JsonString, std::string, locationBlock, root);
		BIND(jsonObject, KEY_LOCATION_LISTING, JsonBoolean, bool, locationBlock, listing);
		BIND(jsonObject, KEY_LOCATION_CGI, JsonString, std::string, locationBlock, cgi);
	}
	catch (...)
	{
		delete locationBlock;

		throw;
	}

	return (locationBlock);
}
