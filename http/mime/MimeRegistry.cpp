/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeRegistry.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 17:39:30 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/02 12:31:03 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/mime/MimeRegistry.hpp>
#include <stddef.h>
#include <util/json/JsonArray.hpp>
#include <util/json/JsonObject.hpp>
#include <util/json/JsonReader.hpp>
#include <util/json/JsonString.hpp>
#include <util/json/JsonValue.hpp>
#include <util/log/LoggerFactory.hpp>
#include <iostream>
#include <iterator>
#include <list>

Logger &MimeRegistry::LOG = LoggerFactory::get("MIME Registry");

MimeRegistry::MimeRegistry(void) :
		m_mapping(),
		m_reverse_mapping()
{
}

MimeRegistry::MimeRegistry(const map &mapping, const map &reverse_mapping) :
		m_mapping(mapping),
		m_reverse_mapping(reverse_mapping)
{
}

MimeRegistry::MimeRegistry(const MimeRegistry &other) :
		m_mapping(other.m_mapping),
		m_reverse_mapping(other.m_reverse_mapping)
{
}

MimeRegistry::~MimeRegistry(void)
{
	clear();
}

MimeRegistry&
MimeRegistry::operator =(const MimeRegistry &other)
{
	if (this != &other)
	{
		m_mapping = other.m_mapping;
		m_reverse_mapping = other.m_reverse_mapping;
	}

	return (*this);
}

void
MimeRegistry::clearMapping(map &mapping)
{
	if (mapping.empty())
		return;

	for (const_iterator it = mapping.begin(); it != mapping.end(); it++)
//		delete it->second;

	return (mapping.clear());
}

void
MimeRegistry::add(const Mime &mime)
{
	const Mime *old;

	if ((old = findByMimeType(mime.type())) != NULL)
		delete old;

	m_mapping[mime.type()] = new Mime(mime);

	Mime::iterator it = mime.extensions().begin();
	Mime::iterator ite = mime.extensions().end();
	while (it != ite)
	{
		const std::string &extension = *it;

		if ((old = findByFileExtension(extension)) != NULL)
			delete old;

		m_reverse_mapping[extension] = new Mime(mime);

		it++;
	}
}

void
MimeRegistry::clear(void)
{
	clearMapping(m_mapping);
	clearMapping(m_reverse_mapping);
}

const Mime*
MimeRegistry::MimeRegistry::findByMimeType(const std::string &type) const
{
	const_iterator it = m_mapping.find(type);

	if (it == m_mapping.end())
		return (NULL);

	return (it->second);
}

const Mime*
MimeRegistry::findByFileExtension(const std::string &type) const
{
	const_iterator it = m_reverse_mapping.find(type);

	if (it == m_reverse_mapping.end())
		return (NULL);

	return (it->second);
}

void
MimeRegistry::loadFromFile(const std::string &path) throw (IOException)
{
	JsonObject *object = JsonReader::fromFile(path).readObject();

	for (JsonObject::iterator it = object->begin(); it != object->end(); it++)
	{
		const std::string &mimeType = it->first;
		JsonValue *jsonValue = it->second;

		if (jsonValue->type() != JsonValue::TYPE_ARRAY)
		{
			LOG.warn() << "not an array: " << jsonValue->typeString() << " (key: " << mimeType << ")" << std::endl;
			continue;
		}

		JsonArray *jsonArray = jsonValue->cast<JsonArray>();

		std::list<std::string> extensions;

		size_t index = 0;
		for (JsonArray::iterator ait = jsonArray->begin(); ait != jsonArray->end(); ait++)
		{
			jsonValue = *ait; /* Recycled variable */

			if (jsonValue->type() != JsonValue::TYPE_STRING)
			{
				LOG.warn() << "not a string (key: " << it->first << ", index: " << index << ")" << std::endl;
				continue;
			}

			extensions.push_back(*(jsonValue->cast<JsonString>())); // TODO Safe check?

			index++;
		}

		if (extensions.empty())
		{
			LOG.warn() << "no usable data (or empty, key: " << mimeType << ")" << std::endl;
			continue;
		}

		add(Mime(mimeType, extensions));
	}

	delete object;

	LOG.debug() << "Loaded " << size() << " MIME(s)" << std::endl;
}

size_t
MimeRegistry::size()
{
	return (m_mapping.size());
}
