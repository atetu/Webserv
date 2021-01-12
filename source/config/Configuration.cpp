/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/12 16:48:53 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/auth/BasicAuthBlock.hpp>
#include <config/block/auth/BearerAuthBlock.hpp>
#include <config/block/container/CustomErrorMap.hpp>
#include <config/block/CGIBlock.hpp>
#include <config/block/LocationBlock.hpp>
#include <config/block/MimeBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <config/exceptions/ConfigurationBindException.hpp>
#include <config/exceptions/ConfigurationValidateException.hpp>
#include <exception/IllegalStateException.hpp>
#include <http/mime/Mime.hpp>
#include <json/JsonArray.hpp>
#include <json/JsonBoolean.hpp>
#include <json/JsonNumber.hpp>
#include <json/JsonReader.hpp>
#include <json/JsonString.hpp>
#include <json/JsonValue.hpp>
#include <libs/ft.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <stddef.h>
#include <unit/DataSize.hpp>
#include <util/Convert.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <util/helper/JsonBinderHelper.hpp>
#include <util/Optional.hpp>
#include <cctype>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <utility>

Logger &Configuration::LOG = LoggerFactory::get("Configuration");
Configuration *Configuration::INSTANCE = NULL;

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
	DeleteHelper::pointer<MimeRegistry>(m_mimeRegistry);
	DeleteHelper::pointer<RootBlock>(m_rootBlock);
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

void
Configuration::setInstance(Configuration *configuration)
{
	if (INSTANCE == configuration)
		return;

	DeleteHelper::pointer<Configuration>(INSTANCE);
	INSTANCE = configuration;
}

Configuration&
Configuration::instance()
{
	if (!INSTANCE)
		throw IllegalStateException("no configuration defined");

	return (*INSTANCE);
}

Configuration*
Configuration::fromJsonFile(const std::string &path, bool ignoreMimeIncludesError)
{
	const JsonObject *jsonObject = &JsonBuilder::rootObject(path);
	RootBlock *rootBlock = NULL;
	MimeRegistry *mimeRegistry = NULL;

	try
	{
		LOG.trace() << "Building ROOT Block..." << std::endl;

		rootBlock = JsonBuilder::buildRootBlock(*jsonObject);
		DeleteHelper::pointer(jsonObject);

		LOG.trace() << "Root Block: " << rootBlock << std::endl;

		LOG.trace() << "Validating..." << std::endl;

		Validator::validate(*rootBlock);

		LOG.trace() << "Filling MIME registry..." << std::endl;

		mimeRegistry = new MimeRegistry();

		if (rootBlock->mimeBlock().present())
		{
			const MimeBlock &mimeBlock = *rootBlock->mimeBlock().get();

			if (mimeBlock.includes().present())
			{
				const std::list<std::string> &includes = mimeBlock.includes().get();

				LOG.trace() << "From " << includes.size() << " file(s)." << std::endl;

				for (std::list<std::string>::const_iterator it = includes.begin(); it != includes.end(); it++)
				{
					const std::string &file = *it;

					LOG.trace() << "Current file: " << file << std::endl;

					try
					{
						mimeRegistry->loadFromFile(file);
					}
					catch (Exception &exception)
					{
						if (ignoreMimeIncludesError)
							LOG.warn() << "Failed to include MIME file: " << file << std::endl;
						else
							throw ConfigurationException("Failed to include MIME file: " + exception.message());
					}
				}
			}

			if (mimeBlock.defines().present())
			{
				const std::list<Mime const*> &defines = mimeBlock.defines().get();

				LOG.trace() << "From " << defines.size() << " define(s)." << std::endl;

				for (std::list<Mime const*>::const_iterator it = defines.begin(); it != defines.end(); it++)
					mimeRegistry->add(*(*it));
			}

			LOG.debug() << "MIME Registry final size: " << mimeRegistry->size() << std::endl;
		}
		else
			LOG.debug() << "MIME Registry is empty (no includes or define found)" << std::endl;

		return (new Configuration(path, *mimeRegistry, *rootBlock));
	}
	catch (...)
	{
		DeleteHelper::pointer(jsonObject);
		DeleteHelper::pointer(rootBlock);
		DeleteHelper::pointer(mimeRegistry);

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
		{
			std::string path = KEY_ROOT;

			BIND(jsonObject, KEY_ROOT_MAX_ACTIVE_CLIENT, JsonNumber, int, rootBlock, maxActiveClient);
		}

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

			mimeBlock->defines(JsonBinderHelper::buildBlocks<Mime, JsonArray>(ipath, object, Mime::builder));
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
		BIND(jsonObject, KEY_CGI_REDIRECT_ERR_TO_OUT, JsonBoolean, bool, cgiBlock, redirectErrToOut);

		if (jsonObject.has(KEY_CGI_EXTENSIONS))
		{
			std::string ipath = path + KEY_DOT KEY_CGI_EXTENSIONS;
			const JsonArray &array = JsonBinderHelper::jsonCast<JsonArray>(ipath, jsonObject.get(KEY_CGI_EXTENSIONS));

			cgiBlock->extensions(JsonBinderHelper::buildCollection<JsonString, std::string>(path, array));
		}

		if (jsonObject.has(KEY_CGI_ENVIRONMENT))
		{
			std::string ipath = path + KEY_DOT KEY_CGI_ENVIRONMENT;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(ipath, jsonObject.get(KEY_CGI_ENVIRONMENT));

			cgiBlock->environment(JsonBinderHelper::buildSimpleMap<JsonString, std::string>(ipath, object));
		}
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
		BIND(jsonObject, KEY_SERVER_DEFAULT, JsonBoolean, bool, serverBlock, isDefault);
		BIND(jsonObject, KEY_SERVER_ROOT, JsonString, std::string, serverBlock, root);
		BIND(jsonObject, KEY_SERVER_LISTING, JsonBoolean, bool, serverBlock, listing);

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

		if (jsonObject.has(KEY_SERVER_METHODS))
		{
			std::string ipath = path + KEY_DOT KEY_SERVER_METHODS;
			const JsonArray &array = JsonBinderHelper::jsonCast<JsonArray>(ipath, jsonObject.get(KEY_SERVER_METHODS));

			serverBlock->methods(JsonBinderHelper::buildCollection<JsonString, std::string>(ipath, array));
		}

		if (jsonObject.has(KEY_SERVER_INDEX_FILES))
		{
			std::string ipath = path + KEY_DOT KEY_SERVER_INDEX_FILES;
			const JsonArray &array = JsonBinderHelper::jsonCast<JsonArray>(ipath, jsonObject.get(KEY_SERVER_INDEX_FILES));

			serverBlock->index(JsonBinderHelper::buildCollection<JsonString, std::string>(ipath, array));
		}

		if (jsonObject.has(KEY_SERVER_AUTH))
		{
			std::string ipath = path + KEY_DOT KEY_SERVER_AUTH;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(ipath, jsonObject.get(KEY_SERVER_AUTH));

			serverBlock->auth(*buildAuthBlock(ipath, object));
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
		//BIND(jsonObject, KEY_LOCATION_MAXBODYSIZE, JsonString, std::string, locationBlock, maxBodySize);

		BIND_CUSTOM(jsonObject, KEY_LOCATION_MAXBODYSIZE, JsonString, std::string,
		{
			try
			{
				locationBlock->maxBodySize(DataSize::parse(value))
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

		if (jsonObject.has(KEY_LOCATION_AUTH))
		{
			std::string ipath = path + KEY_DOT KEY_LOCATION_AUTH;
			const JsonObject &object = JsonBinderHelper::jsonCast<JsonObject>(ipath, jsonObject.get(KEY_LOCATION_AUTH));

			locationBlock->auth(*buildAuthBlock(ipath, object));
		}
	}
	catch (...)
	{
		delete locationBlock;
		throw;
	}

	return (locationBlock);
}

AuthBlock*
Configuration::JsonBuilder::buildAuthBlock(const std::string &path, const JsonObject &jsonObject)
{
	AuthBlock *authBlock = NULL;

	try
	{
		std::string ipath = path + KEY_DOT KEY_AUTH_TYPE;

		if (!jsonObject.has(KEY_AUTH_TYPE))
			throw ConfigurationBindException("auth require a type (" + ipath + ")");

		std::string type = JsonBinderHelper::jsonCast<JsonString>(ipath, jsonObject.get(KEY_AUTH_TYPE));

		if (type == BasicAuthBlock::TYPE)
		{
			ipath = path + KEY_DOT KEY_AUTH_BASIC_USER;

			if (!jsonObject.has(KEY_AUTH_BASIC_USER))
				throw ConfigurationBindException("`basic` authentication require a user (" + ipath + ")");

			std::string user = JsonBinderHelper::jsonCast<JsonString>(ipath, jsonObject.get(KEY_AUTH_BASIC_USER));

			BasicAuthBlock *basicAuthBlock = new BasicAuthBlock(user);
			authBlock = basicAuthBlock;

			BIND(jsonObject, KEY_AUTH_BASIC_PASSWORD, JsonString, std::string, basicAuthBlock, password);
		}
		else if (type == BearerAuthBlock::TYPE)
		{
			ipath = path + KEY_DOT KEY_AUTH_BEARER_TOKEN;

			if (!jsonObject.has(KEY_AUTH_BEARER_TOKEN))
				throw ConfigurationBindException("`bearer` authentication require a token (" + ipath + ")");

			std::string token = JsonBinderHelper::jsonCast<JsonString>(ipath, jsonObject.get(KEY_AUTH_BEARER_TOKEN));

			authBlock = new BearerAuthBlock(token);
		}
		else
			throw ConfigurationBindException("only `basic` or `bearer` authentication supported (" + ipath + ")");

		BIND(jsonObject, KEY_AUTH_REALM, JsonString, std::string, authBlock, realm);
	}
	catch (...)
	{
		if (authBlock)
			delete authBlock;

		throw;
	}

	return (authBlock);
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

#include <limits.h>

void
Configuration::Validator::validate(const RootBlock &rootBlock)
{
	typedef std::list<const ServerBlock*> slist;
	typedef std::list<const LocationBlock*> llist;

	if (rootBlock.maxActiveClient().present())
	{
		long n = rootBlock.maxActiveClient().get();

		if (n < 1)
			throw ConfigurationValidateException("maxActiveClient is under one (<= 1)");
	}

	if (rootBlock.serverBlocks().present())
	{
		const slist &serverBlocks = rootBlock.serverBlocks().get();

		if (serverBlocks.empty())
			throw ConfigurationValidateException("Server list is empty.");

		for (slist::const_iterator sit = serverBlocks.begin(); sit != serverBlocks.end(); sit++)
		{
			const ServerBlock &serverBlock = *(*sit);
			
			if (serverBlock.host().present())
			{
				std::string host = serverBlock.host().get();
				size_t found;
				int loop = 4;
				while (loop--)
				{
					if (((found = host.find(".")) != std::string::npos) || loop == 0)
					{
						std::string subHost = host.substr(0, found);
						host.erase(0, found + 1);
//						std::cout << subHost << std::endl;
						int res;
						if (!subHost.empty() && subHost.size() <= 3)
						{
							size_t i = -1;
							while (++i < subHost.size())
							{
//								std::cout << subHost[i] << std::endl;
								if (!(ft::isdigit(subHost[i])))
									throw ConfigurationValidateException("Host value should only be made of digits.");
							}
						}
						else
						{
							throw ConfigurationValidateException("Host value should be made of 4 units, made up of 1 to 3 digits maximum, separated by dots.");
						}
						if (!((res = ft::atoi(subHost.c_str())) >= 0 && res <= 255))
							throw ConfigurationValidateException("Host value should be made of 4 numbers, ranged between 0 and 255");
					}
					else
						throw ConfigurationValidateException("Host value should be made of 4 units, made up of 1 to 3 digits maximum, separated by dots2.");
				}
			}

			if (serverBlock.locations().present())
			{
				const llist &locationBlocks = serverBlock.locations().get();

				for (llist::const_iterator lit = locationBlocks.begin(); lit != locationBlocks.end(); lit++)
				{
					const LocationBlock &locationBlock = *(*lit);

					if (locationBlock.cgi().present())
					{
						const std::string &cgi = locationBlock.cgi().get();

						if (!rootBlock.hasCGI(cgi))
							throw ConfigurationValidateException("Undefined CGI with name: " + cgi);

						const CGIBlock &cgiBlock = rootBlock.getCGI(cgi);

						if (!cgiBlock.path().present())
							throw ConfigurationValidateException("Used CGI '" + cgiBlock.name() + "' does not have a defined path.");

						if (!cgiBlock.exists())
							LOG.warn() << "CGI: " << cgiBlock.name() << ": " << cgiBlock.path().get() << ": stat() failed. (file does not exists?)" << std::endl;
					}
				}
			}
		}
	}
	else
		throw ConfigurationValidateException("No server slist found.");
}
