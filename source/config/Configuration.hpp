/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 16:19:59 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP_
# define CONFIGURATION_HPP_

#include <config/block/RootBlock.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <json/JsonObject.hpp>
#include <string>

// TODO: All blocks need deep-copy, same goes for mime registry
// Configuration should not be passed around like a normal object, but must be kept as singleton if possible

#define KEY_DOT "."
#define KEY_ROOT "<root>"
#define KEY_ROOT_MAX_ACTIVE_CLIENT "maxActiveClient"
#define KEY_ROOT_ROOT "root"
#define KEY_ROOT_MIME "mime"
#define KEY_ROOT_CGI "cgi"
#define KEY_ROOT_SERVERS "servers"
#define KEY_MIME_INCLUDES "includes"
#define KEY_MIME_DEFINE "define"
#define KEY_CGI_PATH "path"
#define KEY_CGI_REDIRECT_ERR_TO_OUT "redirectErrToOut"
#define KEY_CGI_EXTENSIONS  "extensions"
#define KEY_CGI_ENVIRONMENT  "environment"
#define KEY_SERVER_PORT "port"
#define KEY_SERVER_HOST "host"
#define KEY_SERVER_DEFAULT "default"
#define KEY_SERVER_NAME "name"
#define KEY_SERVER_MAXBODYSIZE "maxBodySize"
#define KEY_SERVER_ROOT "root"
#define KEY_SERVER_LOCATIONS "locations"
#define KEY_SERVER_ERRORS "errors"
#define KEY_SERVER_METHODS "methods"
#define KEY_SERVER_LISTING "listing"
#define KEY_SERVER_INDEX_FILES "index"
#define KEY_SERVER_AUTH "auth"
#define KEY_LOCATION_METHODS "methods"
#define KEY_LOCATION_ALIAS "alias"
#define KEY_LOCATION_ROOT "root"
#define KEY_LOCATION_LISTING "listing"
#define KEY_LOCATION_INDEX_FILES "index"
#define KEY_LOCATION_CGI "cgi"
#define KEY_LOCATION_AUTH "auth"
#define KEY_AUTH_TYPE "type"
#define KEY_AUTH_REALM "realm"
#define KEY_AUTH_BASIC_USER "user"
#define KEY_AUTH_BASIC_PASSWORD "password"
#define KEY_AUTH_BEARER_TOKEN "token"

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
		fromJsonFile(const std::string &path, bool ignoreMimeIncludesError);

	private:
		class JsonBuilder
		{
			public:
				static const JsonObject&
				rootObject(const std::string &filepath);

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

				static AuthBlock*
				buildAuthBlock(const std::string &path, const JsonObject &jsonObject);

				static CustomErrorMap
				buildCustomErrorMap(const std::string &path, const JsonObject &jsonObject);
		};

		class Validator
		{
			public:
				static void
				validate(const RootBlock &rootBlock);
		};
};

#endif /* CONFIGURATION_HPP_ */
