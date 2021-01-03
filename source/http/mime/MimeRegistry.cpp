/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeRegistry.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 17:39:30 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 16:22:30 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/mime/MimeRegistry.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <json/JsonReader.hpp>
#include <log/LoggerFactory.hpp>
#include <iostream>
#include <list>
#include <utility>

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
		copyMapping(other.m_mapping, m_mapping);
		copyMapping(other.m_reverse_mapping, m_reverse_mapping);
	}

	return (*this);
}

void
MimeRegistry::copyMapping(const map &from, map &to)
{
	clearMapping(to);

	for (const_iterator it = from.begin(); it != from.end(); it++)
		to.insert(to.end(), std::make_pair(it->first, new Mime(*(it->second))));
}

void
MimeRegistry::clearMapping(map &mapping)
{
	if (mapping.empty())
		return;

	for (const_iterator it = mapping.begin(); it != mapping.end(); it++)
		delete it->second;

	mapping.clear();
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

		if ((old = findByFileExtension(extension)) != NULL) // TODO Make merging instead of replace
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
MimeRegistry::loadFromFile(const std::string &path)
{
	JsonObject *object = JsonReader::fromFile(path).readObject();

	try
	{
		loadFromJson("<root: " + path + ">", *object);

		LOG.debug() << "Loaded " << size() << " MIME(s)" << std::endl;

		delete object;
	}
	catch (...)
	{
		delete object;
		throw;
	}
}

void
MimeRegistry::loadFromJson(const std::string &path, const JsonObject &jsonObject)
{
	std::list<Mime const*> mimes = Mime::builder(path, jsonObject);

	for (std::list<Mime const*>::iterator it = mimes.begin(); it != mimes.end(); it++)
		add(*(*it));

	DeleteHelper::pointers(mimes);
}

size_t
MimeRegistry::size() const
{
	return (m_mapping.size());
}
