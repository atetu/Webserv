/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/02 12:29:26 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/errno.h>
#include <fstream>
#include <sstream>
#include <config/Configuration.hpp>
#include <exception/IOException.hpp>
#include <util/ContainerBuilder.hpp>
#include <util/unit/DataSize.hpp>
#include <util/unit/DataUnit.hpp>
#include <util/json/Json.hpp>

#define BIND(key, jsonType, type, apply)                                            \
			if (server->has(key))                                                        \
			{                                                                            \
				JsonValue *jsonValue = server->get(key);                                    \
																						\
				if (!jsonValue->instanceOf<jsonType>())                                    \
					throw Exception("Server " key " is not a " #type);                    \
																						\
				type value = *(jsonValue->cast<jsonType>());                                \
																						\
				do {                                                                    \
					apply;                                                                \
				} while (0);                                                            \
			}

Configuration::Configuration() :
		m_file(""),
		m_servers(),
		m_mimeRegistry(),
		m_rootBlock(),
		m_mimeBlock()
{
}

Configuration::Configuration(const std::string &file) :
		m_file(file),
		m_servers(),
		m_mimeRegistry(),
		m_rootBlock(),
		m_mimeBlock()
{
}

Configuration::Configuration(const Configuration &other) :
		m_file(other.m_file),
		m_servers(other.m_servers),
		m_mimeRegistry(other.m_mimeRegistry),
		m_rootBlock(other.m_rootBlock),
		m_mimeBlock(other.m_mimeBlock)
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
		m_servers = other.m_servers;
		m_mimeRegistry = other.m_mimeRegistry;
		m_rootBlock = other.m_rootBlock;
		m_mimeBlock = other.m_mimeBlock;
	}

	return (*this);
}

Configuration&
Configuration::root(RootBlock &rootBlock)
{
	m_rootBlock = rootBlock;
	
	return (*this);
}

Configuration&
Configuration::build()
{
	return (*this);
}

const std::string&
Configuration::getFile(void) const
{
	return (m_file);
}

const std::vector<ServerBlock>&
Configuration::getServers(void) const
{
	return (m_servers);
}

const MimeRegistry&
Configuration::getMimeRegistry(void) const
{
	return (m_mimeRegistry);
}

const MimeBlock&
Configuration::getMimeBlock(void) const
{
	return (m_mimeBlock);
}

const RootBlock&
Configuration::getRootBlock(void) const
{
	return (m_rootBlock);
}

Configuration Configuration::setMime()
{
	m_mimeRegistry.loadFromFile("mime.json");
	return (*this);
}

Configuration
Configuration::fromJsonFile(const std::string &path) throw (IOException)
{
	JsonObject *object = JsonReader::fromFile(path).readObject();

	if (!object->instanceOf<JsonObject>())
    	throw Exception("Expected JSON Object, but got: " + object->typeString());
    
	JsonObject *root = object->cast<JsonObject>(); // pourquoi le caster?

	RootBlock rootBlock;

	if (root->has("servers"))
	{
		JsonValue *serversJsonValue = root->get("servers");

		if (!serversJsonValue->instanceOf<JsonArray>())
			throw Exception("Servers list is not an array");
			
		JsonArray *servers = serversJsonValue->cast<JsonArray>();
		
		JsonArray::iterator it = servers->begin();
		JsonArray::iterator ite = servers->end();
		
		std::list<ServerBlock> serverBlockList;
		
		while (it != ite)
		{
			ServerBlock serverBlock;
			
			if (!(*it)->instanceOf<JsonObject>())
				throw Exception("Server is not an object");
			
			JsonObject *server = (*it)->cast<JsonObject>();
			
			
			BIND("port", JsonNumber, int, 
				{serverBlock.port(value);
			});
			
			BIND("host", JsonString, std::string, {
				serverBlock.host(value);
			});
			
			if (server->has("name"))
			{
				JsonValue *jsonValue = server->get("name");
				if (jsonValue->instanceOf<JsonString>())
				{
					serverBlock.names(ContainerBuilder<std::string, std::list<std::string> >().
					add(*(jsonValue->cast<JsonString>()))
					.build());
				}
				else if (jsonValue->instanceOf<JsonArray>())
				{
					JsonArray *names = jsonValue->cast<JsonArray>();
					
					JsonArray::iterator it = names->begin();
					JsonArray::iterator ite = names->end();
					std::list<std::string> serverNames;
					while (it != ite)
					{
						if (!(*it)->instanceOf<JsonString>())
							throw Exception("Server name is not a string value (in array)");
						serverNames.push_back(*((*it)->cast<JsonString>()));
						it++;
					}
					
					if (serverNames.empty())
						throw Exception("Server name array provided, but empty");
					
					serverBlock.names(serverNames);
				}
			}
			
			//BIND("default", JsonBoolean, bool, // on le met ou default?
			//	{serverBlock.port(value);
			//});
			
			BIND("maxBodySize", JsonString, std::string, 
				{serverBlock.maxBodySize(DataSize::parse(value));
			});
			

			if (server->has("errorPages"))
			{
				JsonValue *errorValues = server->get("errorPages");

				if (!errorValues->instanceOf<JsonObject>())
    				throw Exception("Expected JSON Object, but got: " + object->typeString());
    
				JsonObject *error = errorValues->cast<JsonObject>();

				JsonObject::iterator error_it = error->begin();
				JsonObject::iterator error_ite = error->end();
				
				std::vector<std::string> errorNames;
				std::vector<ErrorPageBlock> errorBlocks;
				
				while (error_it != error_ite)
				{	
					errorNames.push_back(error_it->first);
					error_it++;
				}
				for (std::vector<std::string>::iterator vec_it = errorNames.begin(); vec_it != errorNames.end() ; ++vec_it)
				{
					std::string::const_iterator str_it = (*vec_it).begin();
   					while (str_it != (*vec_it).end() && std::isdigit(*str_it))
						++str_it;
					if (str_it != (*vec_it).end())
						throw Exception("Number excepted for error key");
					
					std::istringstream iss(*vec_it);
					int n;
					iss >> n;
				
					ErrorPageBlock errorBlockElement;
					errorBlockElement.code(n);
					
					JsonValue *errorFile = error->get(*vec_it);
					if (!errorFile->instanceOf<JsonString>())
						throw Exception("Error file is not a string value (in object)");
				
					errorBlockElement.path(*(errorFile->cast<JsonString>()));

					errorBlocks.push_back(errorBlockElement);
				}
				serverBlock.errors(errorBlocks);
		
			}
			
			BIND("root", JsonString, std::string,   // pas sure vu que c'est apres une virgule
				{serverBlock.root(value);
			});
			

			if (server->has("locations"))
			{
				JsonValue *object = server->get("locations");

				if (!object->instanceOf<JsonObject>())
    				throw Exception("Expected JSON Object, but got: " + object->typeString());
    
				JsonObject *locations = object->cast<JsonObject>(); // pourquoi le caster?
				
				JsonObject::iterator locations_it = locations->begin();
				JsonObject::iterator locations_ite = locations->end();
				
				std::vector<std::string> locationsNames;
				std::vector<LocationBlock> locationBlocks;
				
				while (locations_it != locations_ite)
				{	
					locationsNames.push_back(locations_it->first);
					locations_it++;
				}
				for(std::vector<std::string>::iterator vec_it = locationsNames.begin(); vec_it != locationsNames.end() ; ++vec_it)
				{
					std::string locationPath;
					
					if(locations->has(*vec_it))
						locationPath = *vec_it;
										
					LocationBlock LocationBlockElement(locationPath);
					
					JsonValue *jsonValue = locations->get(locationPath);
					if (!jsonValue->instanceOf<JsonObject>())
						throw Exception("Json Object expected for location elements");
					
					JsonObject *LocationObject = jsonValue->cast<JsonObject>();
					
					if (LocationObject->has("methods"))
					{
						JsonValue *jsonValue = LocationObject->get("methods");
					
						if (!jsonValue->instanceOf<JsonArray>())
						{
							throw Exception("Expected JSON Array for methods, but got: " + jsonValue->typeString());
						}
						
						JsonArray *methods = jsonValue->cast<JsonArray>();
							
						JsonArray::iterator method_it = methods->begin();
						JsonArray::iterator method_ite = methods->end();
						
						std::vector<std::string> allowedMethods;
							
						while (method_it != method_ite)
						{
							if (!(*method_it)->instanceOf<JsonString>())
								throw Exception("Method is not a string value (in array)");
							allowedMethods.push_back(*((*method_it)->cast<JsonString>()));
							method_it++;
						}
							
						if (allowedMethods.empty())
							throw Exception("Method array provided, but empty");
						
						LocationBlockElement.methods(allowedMethods);
					}
						
					if (LocationObject->has("alias"))
					{
						JsonValue *jsonValue = LocationObject->get("alias");
					
						if (!jsonValue->instanceOf<JsonString>())
							throw Exception("Expected JSON String for alias, but got: " + jsonValue->typeString());
											
						LocationBlockElement.alias(*(jsonValue->cast<JsonString>()));
					}

					if (LocationObject->has("root"))
					{
						JsonValue *jsonValue = LocationObject->get("root");
					
						if (!jsonValue->instanceOf<JsonString>())
							throw Exception("Expected JSON String for root, but got: " + jsonValue->typeString());
												
						LocationBlockElement.root(*(jsonValue->cast<JsonString>()));
					}

					if (LocationObject->has("listing"))
					{
						JsonValue *jsonValue = LocationObject->get("listing");
						
						if (!jsonValue->instanceOf<JsonBoolean>())
							throw Exception("Expected JSON Boolean for listing, but got: " + jsonValue->typeString());
												
						LocationBlockElement.listing(*(jsonValue->cast<JsonBoolean>()));
					}
						
					if (LocationObject->has("index"))
					{
						JsonValue *jsonValue = LocationObject->get("index");
					
						if (!jsonValue->instanceOf<JsonArray>())
							throw Exception("Expected JSON Array for index, but got: " + jsonValue->typeString());
										
						JsonArray *index = jsonValue->cast<JsonArray>();
													
						JsonArray::iterator index_it = index->begin();
						JsonArray::iterator index_ite = index->end();
						
						std::vector<std::string> indexFiles;
							
						while (index_it != index_ite)
						{
							if (!(*index_it)->instanceOf<JsonString>())
								throw Exception("Index is not a string value (in array)");
							indexFiles.push_back(*((*index_it)->cast<JsonString>()));
							index_it++;
						}
						
						if (indexFiles.empty())
							throw Exception("Index array provided, but empty");
							
						LocationBlockElement.index(indexFiles);
					}

					if (LocationObject->has("uploads"))
					{
						JsonValue *jsonValue = LocationObject->get("uploads");
						if (!jsonValue->instanceOf<JsonObject>())
    						throw Exception("Expected JSON Object, but got: " + jsonValue->typeString());
					}
					
					if (LocationObject->has("cgi"))
					{
						JsonValue *jsonValue = LocationObject->get("cgi");
						if (!jsonValue->instanceOf<JsonString>())
    						throw Exception("Expected JSON String, but got: " + jsonValue->typeString());
						LocationBlockElement.cgi(*(jsonValue->cast<JsonString>()));
					}
					
					locationBlocks.push_back(LocationBlockElement);
				}
				serverBlock.locations(locationBlocks);
			}
			
			serverBlockList.push_back(serverBlock);
			it++;
			
		}
		
		rootBlock.server(serverBlockList);
	}
	
	std::list<CGI> CgiBlockList;
	
	if (root->has("cgi"))
	{
		JsonValue *cgiJsonValue = root->get("cgi");
		if (!cgiJsonValue->instanceOf<JsonObject>())
			throw Exception("CGI list is not an object");
			
		JsonObject *cgiObject = cgiJsonValue->cast<JsonObject>();
		
		JsonObject::iterator cgi_it = cgiObject->begin();
		JsonObject::iterator cgi_ite = cgiObject->end();
	
		std::vector<std::string> CGINames;
					
		while (cgi_it != cgi_ite)
		{	
			CGINames.push_back(cgi_it->first);
			cgi_it++;
		}
		for(std::vector<std::string>::iterator vec_it = CGINames.begin(); vec_it != CGINames.end() ; ++vec_it)
		{
			std::string cgiKey;
			
			if(cgiObject->has(*vec_it))
				cgiKey = *vec_it;
									
			CGI CgiBlockElement(cgiKey);
			
			JsonValue *jsonValue = cgiObject->get(cgiKey);
			
			if (!jsonValue->instanceOf<JsonObject>())
				throw Exception("Json Object expected for CGI elements");
					
			JsonObject *cgiValueObject = jsonValue->cast<JsonObject>();
								
			if (cgiValueObject->has("path"))
			{
				JsonValue *jsonValue = cgiValueObject->get("path");
				
				if (!jsonValue->instanceOf<JsonString>())
					throw Exception("Expected JSON String for cgi path, but got: " + jsonValue->typeString());

				CgiBlockElement.path(*(jsonValue->cast<JsonString>()));
			}
			
			else
				throw Exception("No path given for CGI Object");
			
			CgiBlockList.push_back(CgiBlockElement);
		}								
		
		rootBlock.cgi(CgiBlockList);
	}
	
	if (root->has("mime"))
	{
		JsonValue *mimeJsonValue = root->get("mime");
		
		if (!mimeJsonValue->instanceOf<JsonObject>())
			throw Exception("CGI list is not an object");
			
		JsonObject *mimeObject = mimeJsonValue->cast<JsonObject>();

		MimeBlock mimeBlock;
		
		if (mimeObject->has("includes"))
		{
			JsonValue *jsonValue = mimeObject->get("includes");
		
			if (jsonValue->instanceOf<JsonArray>())
			{
				JsonArray *includes = jsonValue->cast<JsonArray>();
					
				JsonArray::iterator it = includes->begin();
				JsonArray::iterator ite = includes->end();
				
				std::vector<std::string> includesElements;
				
				while (it != ite)
				{
					if (!(*it)->instanceOf<JsonString>())
						throw Exception("Include element is not a string value (in array)");
					includesElements.push_back(*((*it)->cast<JsonString>()));
					it++;
				}
				if (includesElements.empty())
					throw Exception("Includes array provided, but empty");
						
				mimeBlock.includes(includesElements);
			}
		}
		
		if (mimeObject->has("define"))
		{
			JsonValue *jsonValue = mimeObject->get("define");
		
			if (jsonValue->instanceOf<JsonObject>())
			{
				JsonObject *defineObject = jsonValue->cast<JsonObject>();
					
				JsonObject::iterator it = defineObject->begin();
				JsonObject::iterator ite = defineObject->end();
				
				std::vector<Mime> definesElements;

				std::vector<std::string> defineNames;
				
				while (it != ite)
				{
				//	if (!((&(it->first))->instanceOf<JsonString>()))
					// 	throw Exception("Define element is not a string value (in array)");
					defineNames.push_back(it->first);
					it++;
				}

				for(std::vector<std::string>::iterator vec_it = defineNames.begin(); vec_it != defineNames.end() ; ++vec_it)
				{
					std::string defineKey;
			
					defineKey = *vec_it;
			
					JsonValue *jsonValue = defineObject->get(defineKey);
			
					if (!jsonValue->instanceOf<JsonArray>())
						throw Exception("Json Array expected for define elements in mimes");
					
					JsonArray *defineValue = jsonValue->cast<JsonArray>();
					
					JsonArray::iterator define_it = defineValue->begin();
					JsonArray::iterator define_ite = defineValue->end();
					
					std::list<std::string> defineValueList;
					
					while (define_it != define_ite)
					{
						if (!(*define_it)->instanceOf<JsonString>())
							throw Exception("Extension is not a string value (in array)");
						defineValueList.push_back(*((*define_it)->cast<JsonString>()));
						define_it++;
					}

					if (defineValueList.empty())
						throw Exception("Define Value array provided, but empty");
						
					Mime MimeElement(defineKey, defineValueList);
					
					definesElements.push_back(MimeElement);
				}
				mimeBlock.define(definesElements);
			}

		}

	}	
		
		
	
				// other keys
			
			// if (!serverBlock.port().present())
		//		throw Exception("No name has been specified for server");
				
		
	
	

	

	return (Configuration(path)
		.root(rootBlock)
		.setMime()
		.build());
}