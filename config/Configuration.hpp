/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/09 11:25:39 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP_
# define CONFIGURATION_HPP_

#include <config/block/RootBlock.hpp>
#include <config/exceptions/ConfigurationBindException.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <util/Convert.hpp>
#include <util/json/JsonArray.hpp>
#include <util/json/JsonObject.hpp>
#include <util/json/JsonReader.hpp>
#include <util/Objects.hpp>
#include <util/Optional.hpp>
#include <iterator>
#include <list>
#include <map>
#include <string>

// TODO: All blocks need deep-copy, same goes for mime registry
// Configuration should not be passed around like a normal object, but must be kept as singleton if possible

#define KEY_DOT "."
#define KEY_ROOT "<root>"
#define KEY_ROOT_ROOT "root"
#define KEY_ROOT_MIME "mime"
#define KEY_ROOT_CGI "cgi"
#define KEY_ROOT_SERVERS "servers"
#define KEY_MIME_INCLUDES "includes"
#define KEY_MIME_DEFINE "define"
#define KEY_CGI_PATH "path"
#define KEY_SERVER_PORT "port"
#define KEY_SERVER_HOST "host"
#define KEY_SERVER_NAME "name"
#define KEY_SERVER_MAXBODYSIZE "maxBodySize"
#define KEY_SERVER_ROOT "root"
#define KEY_SERVER_LOCATIONS "locations"
#define KEY_SERVER_ERRORS "errors"
#define KEY_LOCATION_METHODS "methods"
#define KEY_LOCATION_ALIAS "alias"
#define KEY_LOCATION_ROOT "root"
#define KEY_LOCATION_LISTING "listing"
#define KEY_LOCATION_INDEX_FILES "index"
#define KEY_LOCATION_CGI "cgi"

class Configuration
{
	public:
		static Logger &LOG;

	private:
		std::string m_file;
		const MimeRegistry *m_mimeRegistry;
		const RootBlock *m_rootBlock;

	public:
		Configuration(void);
		Configuration(const std::string &file, const MimeRegistry &mimeRegistry, const RootBlock &rootBlock);
		Configuration(const Configuration &other);

		virtual
		~Configuration();

		Configuration&
		operator=(const Configuration &other);

		inline const std::string&
		file(void) const
		{
			return (m_file);
		}

		inline const MimeRegistry&
		mimeRegistry(void) const
		{
			return (*m_mimeRegistry);
		}

		inline const RootBlock&
		rootBlock(void) const
		{
			return (*m_rootBlock);
		}

	public:
		static Configuration*
		fromJsonFile(const std::string &path);

		template<typename T>
			static void
			deleteBlocks(Optional<std::list<T const*> > &optional)
			{
				if (optional.present())
				{
					deleteBlocks(optional.get());
					optional.unset();
				}
			}

		template<typename T>
			static void
			deleteBlocks(std::list<T const*> &blocks)
			{
				for (typename std::list<T const*>::iterator it = blocks.begin(); it != blocks.end(); it++)
					delete *it;

				blocks.clear();
			}

		template<typename T>
			static void
			deleteBlock(Optional<T const*> &optional)
			{
				if (optional.present())
				{
					delete optional.get();
					optional.unset();
				}
			}

	private:
		class JsonBuilder
		{
			public:
				static const JsonObject&
				rootObject(const std::string &filepath);

				template<typename T, typename JT>
					static std::list<T const*>
					buildBlocks(const std::string &path, const JsonArray &jsonArray, T*
					(*builder)(const std::string&, const JT&))
					{
						std::list<const T*> blocks;

						try
						{
							int index = 0;
							for (JsonArray::const_iterator it = jsonArray.begin(); it != jsonArray.end(); it++)
							{
								std::string ipath = path + "[" + Convert::toString(index) + "]";
								const JT &jsonType = jsonCast<JT>(ipath, *it);

								blocks.push_back((*builder)(ipath, jsonType));

								index++;
							}
						}
						catch (...)
						{
							Configuration::deleteBlocks<T>(blocks);

							throw;
						}

						return (blocks);
					}

				template<typename T, typename JT>
					static std::list<T const*>
					buildBlocks(const std::string &path, const JsonObject &jsonObject, T*
					(*builder)(const std::string&, const std::string&, const JT&))
					{
						std::list<const T*> blocks;

						try
						{
							for (JsonObject::const_iterator it = jsonObject.begin(); it != jsonObject.end(); it++)
							{
								const std::string &key = it->first;
								std::string ipath = path + KEY_DOT + key;
								const JT &jsonType = jsonCast<JT>(ipath, it->second);

								blocks.push_back((*builder)(ipath, key, jsonType));
							}
						}
						catch (...)
						{
							Configuration::deleteBlocks<T>(blocks);

							throw;
						}

						return (blocks);
					}

				template<typename JT, typename T>
					static std::list<T>
					buildCollection(const std::string &path, const JsonArray &jsonArray)
					{
						std::list<T> items;

						int index = 0;
						for (JsonArray::const_iterator it = jsonArray.begin(); it != jsonArray.end(); it++)
						{
							std::string ipath = path + "[" + Convert::toString(index) + "]";
							const JT &jsonType = jsonCast<JT>(ipath, *it);

							T item = jsonType; /* Automatic type operator cast. */
							items.push_back(item);

							index++;
						}

						return (items);
					}

				static RootBlock*
				buildRootBlock(const JsonObject &jsonObject);

				static MimeBlock*
				buildMimeBlock(const std::string &path, const JsonObject &jsonObject);

				static CGIBlock*
				buildCGIBlock(const std::string &path, const std::string &key, const JsonObject &jsonObject);

				static ServerBlock*
				buildServerBlock(const std::string &path, const JsonObject &jsonObject);

				static LocationBlock*
				buildLocationBlock(const std::string &path, const std::string &key, const JsonObject &jsonObject);

				static Mime*
				buildMime(const std::string &path, const std::string &key, const JsonArray &jsonArray);

				static CustomErrorMap
				buildCustomErrorMap(const std::string &path, const JsonObject &jsonObject);

				template<typename T>
					static const T&
					jsonCast(const std::string &path, const JsonValue *jsonValue)
					{
						Objects::requireNonNull(jsonValue, "jsonValue == null");

						if (!jsonValue->instanceOf<T>())
							throw ConfigurationBindException::uncastable<T>(path, *jsonValue);

						return (*(jsonValue->cast<T>()));
					}
		};
};

#endif /* CONFIGURATION_HPP_ */
