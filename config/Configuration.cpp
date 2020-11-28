/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/28 18:46:55 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/errno.h>
#include <fstream>
#include <sstream>
#include <config/Configuration.hpp>
#include <exception/IOException.hpp>
#include <ContainerBuilder.hpp>
#include <json/Json.hpp>

// Configuration::Configuration() :
// 		m_servers()
// {
// }

// Configuration::Configuration(std::vector<ServerBlock> servers) :
// 		m_servers(servers)
// {
// }

// Configuration::Configuration(const Configuration &other) :
// 		m_servers(other.m_servers)
// {
// }

Configuration::Configuration(const std::string &file) :
		m_file(file)
{
}

Configuration::~Configuration()
{
}

// Configuration&
// Configuration::operator=(const Configuration &other)
// {
// 	if (this != &other)
// 		m_servers = other.m_servers;

// 	return (*this);
// }

// const std::vector<ServerBlock>&
// Configuration::servers(void) const
// {
// 	return (m_servers);
// }

Configuration Configuration::setMime()
{
	m_mimeRegistry.loadFromFile("mime.json");
	return (*this);
}

Configuration fromJsonFile(const std::string &path) throw (IOException)
{
	JsonObject *object = JsonReader::fromFile(path).readObject();

	if (!object->instanceOf<JsonObject>())
    	throw Exception("Expected JSON Object, but got: " + object->typeString());
    
	JsonObject *root = object->cast<JsonObject>(); // pourquoi le caster?

	if (root->has("servers"))
	{
		JsonValue *serversJsonValue = root->get("servers");

		if (!serversJsonValue->instanceOf<JsonArray>())
			throw Exception("Servers list is not an array");
			
		JsonArray *servers = serversJsonValue->cast<JsonArray>();
		
		JsonArray::iterator it = servers->begin();
		JsonArray::iterator ite = servers->end();
		
		while (it != ite)
		{
			ServerBlock serverBlock;
			
			if (!(*it)->instanceOf<JsonObject>())
				throw Exception("Server is not an object");
			
			JsonObject *server = (*it)->cast<JsonObject>();
			
			
			JsonValue *portValue;
			
			if (server->has("port"))
			{
				portValue = server->get("port");
				
				if (!portValue->instanceOf<JsonNumber>())
					throw Exception("Server port is not a number");
				
				serverBlock.port(*(portValue->cast<JsonNumber>())); //est ce que c'est ca?
			}
			
			if (server->has("host"))
			{
				JsonValue *hostValue = server->get("host");
				
				if (!portValue->instanceOf<JsonString>())
					throw Exception("Server host is not a string");
				
				serverBlock.host(*(portValue->cast<JsonString>()));
			}
	
			
			if (server->has("name"))
			{
				JsonValue *jsonValue = server->get("name");
				std::list<std::string> serverNames;
				if (jsonValue->instanceOf<JsonString>())
				{
					serverNames.push_back (*(jsonValue->cast<JsonString>()));
					serverBlock.names(serverNames);
				}
				else if (jsonValue->instanceOf<JsonArray>())
				{
					JsonArray *names = jsonValue->cast<JsonArray>();
					
					JsonArray::iterator it = names->begin();
					JsonArray::iterator ite = names->end();
					
				//	std::vector<std::string> serverNames;
					
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
					
				
				if (!jsonValue->instanceOf<JsonString>())
					throw Exception("Server host is not a string");
				
				serverBlock.host(portValue->cast<JsonString>());
			}
			
			BIND("port", JsonNumber, int, {
				serverBlock.port(value);
			});
			
			BIND("host", JsonString, std::string, {
				serverBlock.host(value);
			});
			
			// other keys
			
			if (!serverBlock.port().present())
				throw Exception("No name has been specified for server");
				
			
			it++;
		}
	}

	#define BIND(key, jsonType, type, apply)                                            \
			if (server->has(key))                                                        \
			{                                                                            \
				JsonValue *jsonValue = server.get(key);                                    \
																						\
				if (!jsonValue->instanceOf<jsonType>())                                    \
					throw Exception("Server " key " is not a " #type);                    \
																						\
				type value = jsonValue->cast<jsonType>();                                \
																						\
				do {                                                                    \
					apply;                                                                \
				} while (0);                                                            \
			}
}