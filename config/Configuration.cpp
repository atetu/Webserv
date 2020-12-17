/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/16 13:13:57 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/container/CustomErrorMap.hpp>
#include <config/block/CGIBlock.hpp>
#include <config/block/LocationBlock.hpp>
#include <config/block/MimeBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <http/mime/Mime.hpp>
#include <util/helper/JsonBinderHelper.hpp>
#include <util/json/JsonString.hpp>
#include <util/json/JsonValue.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/unit/DataSize.hpp>
#include <cctype>
#include <iostream>
#include <utility>

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
	delete m_rootBlock;
	delete m_mimeRegistry;
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

Configuration*
Configuration::fromJsonFile(const std::string &path, bool ignoreMimeIncludesError)
{
	const JsonObject &jsonObject = JsonBuilder::rootObject(path);
	RootBlock *rootBlock = NULL;
	MimeRegistry *mimeRegistry = NULL;

	try
	{
		LOG.trace() << "Building ROOT Block..." << std::endl;

		rootBlock = JsonBuilder::buildRootBlock(jsonObject);
		delete &jsonObject;

		LOG.trace() << "Root Block: " << rootBlock << std::endl;

		LOG.trace() << "Validating..." << std::endl;

		// TODO Make validation

		LOG.trace() << "Filling MIME registry..." << std::endl;

		mimeRegistry = new MimeRegistry();

		if (rootBlock->mimeBlock().present())
		{
			const MimeBlock &mimeBlock = *rootBlock->mimeBlock().get();

			if (mimeBlock.includes().present())
			{
				const std::list<std::string> &includes = mimeBlock.includes().get();

				for (std::list<std::string>::const_iterator it = includes.begin(); it != includes.end(); it++)
				{
					try
					{
						mimeRegistry->loadFromFile(path);
					}
					catch (Exception &exception)
					{
						if (ignoreMimeIncludesError)
							LOG.warn() << "Failed to include MIME file: " << *it << std::endl;
						else
							throw ConfigurationException("Failed to include MIME file: " + exception.message());
					}
				}
			}
		}

		return (new Configuration(path, *mimeRegistry, *rootBlock));
	}
	catch (...)
	{
		delete &jsonObject;

		if (rootBlock)
			delete rootBlock;

		if (mimeRegistry)
			delete mimeRegistry;

		throw;
	}
}

#define BIND(object, key, jsonType, type, to, apply) \
			BIND_CUSTOM(object, key, jsonType, type, to->apply((type) value));

#define BIND_CUSTOM(object, key, jsonType, type, apply)                                 \
			if ((object).has(key))                                                      \
			{                                                                           \
				std::string ipath = path + KEY_DOT + key;                               \
				const JsonValue *jsonValue = (object).get(key);                         \
																						\
				type value = JsonBinderHelper::jsonCast<jsonType>(ipath, jsonValue);    \
																						\
				do {                                                                    \
					apply;                                                              \
				} while (0);                                                            \
			}

const JsonObject&
Configuration::JsonBuilder::rootObject(const std::string &filepath)
{
	JsonValue *jsonValue = JsonReader::fromFile(filepath).read();

	try
	{
		return (JsonBinderHelper::jsonCast<JsonObject>(KEY_ROOT, jsonValue));
	}
	catch (...)
	{
		delete jsonValue;
		throw;
	}
}

RootBlock*
Configuration::JsonBuilder::buildRootBlock(const JsonObject &jsonObject)
{
	RootBlock *rootBlock = new RootBlock();

	try
	{
		if (jsonObject.has(KEY_ROOT_ROOT))
		{
			std::string ipath = KEY_ROOT KEY_DOT KEY_ROOT_ROOT;
			const JsonString &string = JsonBinderHelper::jsonCast<JsonString>(ipath, jsonObject.get(KEY_ROOT_ROOT));

			rootBlock->root(string);
		}

		if (jsonObject.has(KEY_ROOT_MIME))
		{
			std::string piath = KEY_ROOT KEY_DOT KEY_ROOT_MIME;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(piath, jsonObject.get(KEY_ROOT_MIME));

			rootBlock->mimeBlock(*buildMimeBlock(piath, object));
		}

		if (jsonObject.has(KEY_ROOT_CGI))
		{
			std::string ipath = KEY_ROOT KEY_DOT KEY_ROOT_CGI;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(ipath, jsonObject.get(KEY_ROOT_CGI));

			rootBlock->cgiBlocks(JsonBinderHelper::buildBlocks<CGIBlock, JsonObject>(ipath, object, &Configuration::JsonBuilder::buildCGIBlock));
		}

		if (jsonObject.has(KEY_ROOT_SERVERS))
		{
			std::string ipath = KEY_ROOT KEY_DOT KEY_ROOT_SERVERS;
			const JsonArray &array = JsonBinderHelper::jsonCast<JsonArray>(ipath, jsonObject.get(KEY_ROOT_SERVERS));

			rootBlock->serverBlocks(JsonBinderHelper::buildBlocks<ServerBlock, JsonObject>(ipath, array, &Configuration::JsonBuilder::buildServerBlock));
		}
	}
	catch (...)
	{
		delete rootBlock;
		throw;
	}

	return (rootBlock);
}

MimeBlock*
Configuration::JsonBuilder::buildMimeBlock(const std::string &path, const JsonObject &jsonObject)
{
	MimeBlock *mimeBlock = new MimeBlock();

	try
	{
		if (jsonObject.has(KEY_MIME_INCLUDES))
		{
			std::string ipath = path + KEY_DOT KEY_MIME_INCLUDES;
			const JsonArray &array = JsonBinderHelper::jsonCast<JsonArray>(ipath, jsonObject.get(KEY_MIME_INCLUDES));

			mimeBlock->includes(JsonBinderHelper::buildCollection<JsonString, std::string>(ipath, array));
		}

		if (jsonObject.has(KEY_MIME_DEFINE))
		{
			std::string ipath = path + KEY_DOT KEY_MIME_DEFINE;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(ipath, jsonObject.get(KEY_MIME_DEFINE));

			mimeBlock->defines(JsonBinderHelper::buildBlocks<Mime, JsonArray>(ipath, object, Configuration::JsonBuilder::buildMime));
		}
	}
	catch (...)
	{
		delete mimeBlock;
		throw;
	}

	return (mimeBlock);
}

CGIBlock*
Configuration::JsonBuilder::buildCGIBlock(const std::string &path, const std::string &key, const JsonObject &jsonObject)
{
	CGIBlock *cgiBlock = new CGIBlock(key);

	try
	{
		BIND(jsonObject, KEY_CGI_PATH, JsonString, std::string, cgiBlock, path);
	}
	catch (...)
	{
		delete cgiBlock;
		throw;
	}

	return (cgiBlock);
}

ServerBlock*
Configuration::JsonBuilder::buildServerBlock(const std::string &path, const JsonObject &jsonObject)
{
	ServerBlock *serverBlock = new ServerBlock();

	try
	{
		BIND(jsonObject, KEY_SERVER_PORT, JsonNumber, int, serverBlock, port);
		BIND(jsonObject, KEY_SERVER_HOST, JsonString, std::string, serverBlock, host);
		BIND(jsonObject, KEY_SERVER_ROOT, JsonString, std::string, serverBlock, root);

		if (jsonObject.has(KEY_SERVER_NAME))
		{
			std::list<std::string> names;

			std::string ipath = path + KEY_DOT KEY_SERVER_NAME;

			const JsonValue *jsonValue = jsonObject.get(KEY_SERVER_NAME);
			if (jsonValue->instanceOf<JsonArray>())
				names = JsonBinderHelper::buildCollection<JsonString, std::string>(ipath, *(jsonValue->cast<JsonArray>()));
			else if (jsonValue->instanceOf<JsonString>())
				names.push_back(jsonValue->cast<JsonString>()->operator std::string());
			else
				throw ConfigurationBindException::uncastable2<JsonArray, JsonString>(ipath, *jsonValue);

			serverBlock->names(names);
		}

		BIND_CUSTOM(jsonObject, KEY_SERVER_MAXBODYSIZE, JsonString, std::string,
		{
			try
			{
				serverBlock->maxBodySize(DataSize::parse(value))
				;
				/* <-- Strange Eclipse bug when in macros. */
			}
			catch (Exception &exception)
			{
				throw Exception(exception.message() + std::string(" (") + path + ")")
				;
				/* <-- Strange Eclipse bug when in macros. */
			}
		});

		if (jsonObject.has(KEY_SERVER_LOCATIONS))
		{
			std::string ipath = path + KEY_DOT KEY_SERVER_LOCATIONS;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(ipath, jsonObject.get(KEY_SERVER_LOCATIONS));

			serverBlock->locations(JsonBinderHelper::buildBlocks<LocationBlock, JsonObject>(ipath, object, &Configuration::JsonBuilder::buildLocationBlock));
		}

		if (jsonObject.has(KEY_SERVER_ERRORS))
		{
			std::string ipath = path + KEY_DOT KEY_SERVER_ERRORS;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(ipath, jsonObject.get(KEY_SERVER_ERRORS));

			serverBlock->errors(buildCustomErrorMap(ipath, object));
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
Configuration::JsonBuilder::buildLocationBlock(const std::string &path, const std::string &key, const JsonObject &jsonObject)
{
	LocationBlock *locationBlock = new LocationBlock(key);

	try
	{
		BIND(jsonObject, KEY_LOCATION_ALIAS, JsonString, std::string, locationBlock, alias);
		BIND(jsonObject, KEY_LOCATION_ROOT, JsonString, std::string, locationBlock, root);
		BIND(jsonObject, KEY_LOCATION_LISTING, JsonBoolean, bool, locationBlock, listing);
		BIND(jsonObject, KEY_LOCATION_CGI, JsonString, std::string, locationBlock, cgi);

		if (jsonObject.has(KEY_LOCATION_METHODS))
		{
			std::string ipath = path + KEY_DOT KEY_LOCATION_METHODS;
			const JsonArray &array = JsonBinderHelper::jsonCast<JsonArray>(ipath, jsonObject.get(KEY_LOCATION_METHODS));

			locationBlock->methods(JsonBinderHelper::buildCollection<JsonString, std::string>(ipath, array));
		}

		if (jsonObject.has(KEY_LOCATION_INDEX_FILES))
		{
			std::string ipath = path + KEY_DOT KEY_LOCATION_INDEX_FILES;
			const JsonArray &array = JsonBinderHelper::jsonCast<JsonArray>(ipath, jsonObject.get(KEY_LOCATION_INDEX_FILES));

			locationBlock->index(JsonBinderHelper::buildCollection<JsonString, std::string>(ipath, array));
		}
	}
	catch (...)
	{
		delete locationBlock;
		throw;
	}

	return (locationBlock);
}

Mime*
Configuration::JsonBuilder::buildMime(const std::string &path, const std::string &key, const JsonArray &jsonArray)
{
	const std::string &type = key;
	const std::list<std::string> extensions = JsonBinderHelper::buildCollection<JsonString, std::string>(path, jsonArray);

	return (new Mime(type, extensions));
}

CustomErrorMap
Configuration::JsonBuilder::buildCustomErrorMap(const std::string &path, const JsonObject &jsonObject)
{
	CustomErrorMap::map map;

	for (JsonObject::const_iterator it = jsonObject.begin(); it != jsonObject.end(); it++)
	{
		long code;
		const std::string key = it->first;

		for (std::string::const_iterator kit = key.begin(); kit != key.end(); kit++)
		{
			if (!std::isdigit(*kit))
				throw ConfigurationBindException("key '" + key + "' must only contains numbers (" + path + ")");
		}

		const JsonString &value = JsonBinderHelper::jsonCast<JsonString>(path + KEY_DOT + key, it->second);

		std::stringstream stream;
		stream << key;
		stream >> code;

		if (code == 0 || (code / 100 != 4 && code / 100 != 5)) // TODO Move to HTTPStatus::isError()
			throw ConfigurationBindException("code '" + Convert::toString(code) + "' must be an error 4xx or 5xx (" + path + ")");

		map.insert(map.end(), std::make_pair(code, value));
	}

	return (CustomErrorMap(map));
}
