/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeaderFields.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 18:39:24 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 16:23:54 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/HTTP.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPValueParser.hpp>
#include <http/mime/Mime.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <util/Convert.hpp>
#include <util/Enum.hpp>

const std::string HTTPHeaderFields::ACCEPT_CHARSETS/*	*/= "Accept-Charsets";
const std::string HTTPHeaderFields::ACCEPT_LANGUAGE/*	*/= "Accept-Language";
const std::string HTTPHeaderFields::ALLOW/*				*/= "Allow";
const std::string HTTPHeaderFields::AUTHORIZATION/*		*/= "Authorization";
const std::string HTTPHeaderFields::CONTENT_LANGUAGE/*	*/= "Content-Language";
const std::string HTTPHeaderFields::CONTENT_LENGTH/*	*/= "Content-Length";
const std::string HTTPHeaderFields::CONTENT_LOCATION/*	*/= "Content-Location";
const std::string HTTPHeaderFields::CONTENT_TYPE/*		*/= "Content-Type";
const std::string HTTPHeaderFields::DATE/*				*/= "Date";
const std::string HTTPHeaderFields::HOST/*				*/= "Host";
const std::string HTTPHeaderFields::LAST_MODIFIED/*		*/= "Last-Modified";
const std::string HTTPHeaderFields::LOCATION/*			*/= "Location";
const std::string HTTPHeaderFields::REFERER/*			*/= "Referer";
const std::string HTTPHeaderFields::RETRY_AFTER/*		*/= "Retry-After";
const std::string HTTPHeaderFields::SERVER/*			*/= "Server";
const std::string HTTPHeaderFields::TRANSFER_ENCODING/*	*/= "Transfer-Encoding";
const std::string HTTPHeaderFields::USER_AGENT/*		*/= "User-Agent";
const std::string HTTPHeaderFields::WWW_AUTHENTICATE/*	*/= "WWW-Authenticate";

const std::string HTTPHeaderFields::MIME_HTML/*	        */= "text/html";

HTTPHeaderFields::HTTPHeaderFields(void) :
		m_storage()
{
}

HTTPHeaderFields::HTTPHeaderFields(map storage) :
		m_storage(storage)
{
}

HTTPHeaderFields::HTTPHeaderFields(const HTTPHeaderFields &other) :
		m_storage(other.m_storage)
{
}

HTTPHeaderFields::~HTTPHeaderFields()
{
}

HTTPHeaderFields&
HTTPHeaderFields::operator=(const HTTPHeaderFields &other)
{
	if (this != &other)
		m_storage = other.m_storage;

	return (*this);
}

HTTPHeaderFields&
HTTPHeaderFields::acceptCharsets(const std::string &value)
{
	return (set(ACCEPT_CHARSETS, value));
}

HTTPHeaderFields&
HTTPHeaderFields::acceptLanguage(const std::string &value)
{
	return (set(ACCEPT_LANGUAGE, value));
}

HTTPHeaderFields&
HTTPHeaderFields::allow(const HTTPMethod &method)
{
	return (allow(method.name()));
}

HTTPHeaderFields&
HTTPHeaderFields::allow(const std::vector<HTTPMethod> &methods)
{
	return (allow(Convert::join(methods)));
}

HTTPHeaderFields&
HTTPHeaderFields::allow(const std::list<HTTPMethod> &methods)
{
	return (allow(Convert::join(methods)));
}

HTTPHeaderFields&
HTTPHeaderFields::allow(const std::string &value)
{
	return (set(ALLOW, value));
}

HTTPHeaderFields&
HTTPHeaderFields::authorization(const std::string &type, const std::string &credentials)
{
	return (authorization(type + " " + credentials));
}

HTTPHeaderFields&
HTTPHeaderFields::authorization(const std::string &value)
{
	return (set(AUTHORIZATION, value));
}

HTTPHeaderFields&
HTTPHeaderFields::contentLanguage(const std::string &value)
{
	return (set(CONTENT_LANGUAGE, value));
}

HTTPHeaderFields&
HTTPHeaderFields::contentLocation(const std::string &value)
{
	return (set(CONTENT_LOCATION, value));
}

HTTPHeaderFields&
HTTPHeaderFields::contentLength(size_t length)
{
	return (set(CONTENT_LENGTH, Convert::toString(length)));
}

HTTPHeaderFields&
HTTPHeaderFields::contentType(const MimeRegistry &registry, const std::string &extension)
{
	const Mime *mime = registry.findByFileExtension(extension);

	if (mime)
		return (contentType(*mime));

	return (*this);
}

HTTPHeaderFields&
HTTPHeaderFields::contentType(const std::string &mimeType)
{
	return (set(CONTENT_TYPE, mimeType));
}

HTTPHeaderFields&
HTTPHeaderFields::contentType(const Mime &mime)
{
	return (set(CONTENT_TYPE, mime.type()));
}

HTTPHeaderFields&
HTTPHeaderFields::date(void)
{
	return (date(HTTPDate::now()));
}

HTTPHeaderFields&
HTTPHeaderFields::date(const HTTPDate &date)
{
	return (this->date(date.format()));
}

HTTPHeaderFields&
HTTPHeaderFields::date(const std::string &value)
{
	return (set(DATE, value));
}

HTTPHeaderFields&
HTTPHeaderFields::host(const std::string &value)
{
	return (set(HOST, value));
}

HTTPHeaderFields&
HTTPHeaderFields::lastModified(const HTTPDate &date)
{
	return (lastModified(date.format()));
}

HTTPHeaderFields&
HTTPHeaderFields::lastModified(const std::string &value)
{
	return (set(LAST_MODIFIED, value));
}

HTTPHeaderFields&
HTTPHeaderFields::location(const std::string &value)
{
	return (set(LOCATION, value));
}

HTTPHeaderFields&
HTTPHeaderFields::referer(const std::string &value)
{
	return (set(REFERER, value));
}

HTTPHeaderFields&
HTTPHeaderFields::retryAfter(size_t seconds)
{
	return (retryAfter(Convert::toString(seconds)));
}

HTTPHeaderFields&
HTTPHeaderFields::retryAfter(const HTTPDate &date)
{
	return (retryAfter(date.format()));
}

HTTPHeaderFields&
HTTPHeaderFields::retryAfter(const std::string &value)
{
	return (set(RETRY_AFTER, value));
}

HTTPHeaderFields&
HTTPHeaderFields::server(const std::string &value)
{
	return (set(SERVER, value));
}

HTTPHeaderFields&
HTTPHeaderFields::transferEncoding(const std::vector<std::string> &encodings)
{
	return (transferEncoding(Convert::join(encodings)));
}

HTTPHeaderFields&
HTTPHeaderFields::transferEncoding(const std::list<std::string> &encodings)
{
	return (transferEncoding(Convert::join(encodings)));
}

HTTPHeaderFields&
HTTPHeaderFields::transferEncoding(const std::string &value)
{
	return (set(TRANSFER_ENCODING, value));
}

HTTPHeaderFields&
HTTPHeaderFields::userAgent(const std::string &value)
{
	return (set(USER_AGENT, value));
}

HTTPHeaderFields&
HTTPHeaderFields::wwwAuthenticate(const std::string &value)
{
	return (set(WWW_AUTHENTICATE, value));
}

HTTPHeaderFields&
HTTPHeaderFields::html(void)
{
	return (contentType(MIME_HTML));
}

HTTPHeaderFields&
HTTPHeaderFields::set(const std::string &key, const std::string &value)
{
	m_storage[key] = value;

	return (*this);
}

HTTPHeaderFields::const_iterator
HTTPHeaderFields::begin(void) const
{
	return (m_storage.begin());
}

HTTPHeaderFields::const_iterator
HTTPHeaderFields::end(void) const
{
	return (m_storage.end());
}

std::string
HTTPHeaderFields::format(void) const
{
	std::string str;

	for (const_iterator it = begin(); it != end(); it++)
		str += it->first + std::string(": ") + it->second + HTTP::CRLF;

	return (str);
}

std::map<std::string, std::string>&
HTTPHeaderFields::storage(void)
{
	return (m_storage);
}

// #if 0
HTTPValueParser*
HTTPHeaderFields::getValueWithWeight(std::string field)
{
	map::iterator it = m_storage.begin();
	map::iterator ite = m_storage.end();

	while (it != ite && it->first != field)
		it++;

	std::string value = it->second;

	HTTPValueParser *valueParser = new HTTPValueParser(value);

	char c;

	while (valueParser->next(c))
		valueParser->consume(c);
	valueParser->consume('\0');

	// std::map<float, std::list<std::string> > *mmap = valueParser->getMap();

	//  std::map<float,  std::list<std::string> >::iterator itt = mmap->begin();
	// std::map<float,  std::list<std::string> >::iterator itte = mmap->end();

	// while (itt != itte)
	// {
	//     std::cout << itt->first << " : " ;
	// std::list<std::string>::iterator it_list =itt->second.begin();
	// std::list<std::string>::iterator ite_list = itt->second.end();

	// while (it_list != ite_list)
	// {

	// 	  std::cout<< "[value] " << *it_list << std::endl;
	//       it_list++;
	// }
	// itt++;
	// }
	return (valueParser);
}

HTTPHeaderFields*
HTTPHeaderFields::create(std::map<std::string, std::string> headerMap)
{
	// std::map<std::string, std::string>::iterator it = headerMap.begin();
	// std::map<std::string, std::string>::iterator ite = headerMap.end();

	HTTPHeaderFields *header = new HTTPHeaderFields(headerMap);

	// while (it != ite)
	// {
	// 	header->set(it->getField(), it->getValue());

	// 	if (it->getField().compare(ACCEPT_CHARSETS) == 0)
	// 	{
	// //		std::string value = it->getValue();
	// //		HTTPValueParser valueParser(value);
	// 		header->acceptCharsets(it->getValue());
	// 	}
	// 	else if (it->getField().compare(ACCEPT_LANGUAGE) == 0)
	// 		header->acceptLanguage(it->getValue());
	// 	else if (it->getField().compare(ALLOW) == 0)
	// 		header->allow(it->getValue());
	// 	else if (it->getField().compare(AUTHORIZATION) == 0)
	// 		header->authorization(it->getValue());
	// 	else if (it->getField().compare(CONTENT_LANGUAGE) == 0)
	// 		header->contentLanguage(it->getValue());
	// 	else if (it->getField().compare(CONTENT_LOCATION) == 0)
	// 		header->contentLocation(it->getValue());
	// 	else if (it->getField().compare(CONTENT_TYPE) == 0)
	// 		header->acceptCharsets(it->getValue());
	// 	else if (it->getField().compare(DATE) == 0)
	// 		header->date(it->getValue());
	// 	else if (it->getField().compare(HOST) == 0)
	// 		header->host(it->getValue());
	// 	else if (it->getField().compare(LAST_MODIFIED) == 0)
	// 		header->lastModified(it->getValue());
	// 	else if (it->getField().compare(LOCATION) == 0)
	// 		header->location(it->getValue());
	// 	else if (it->getField().compare(REFERER) == 0)
	// 		header->referer(it->getValue());
	// 	else if (it->getField().compare(RETRY_AFTER) == 0)
	// 		header->retryAfter(it->getValue());
	// 	else if (it->getField().compare(SERVER) == 0)
	// 		header->server(it->getValue());
	// 	else if (it->getField().compare(TRANSFER_ENCODING) == 0)
	// 		header->transferEncoding(it->getValue());
	// 	else if (it->getField().compare(USER_AGENT) == 0)
	// 		header->userAgent(it->getValue());
	// 	else if (it->getField().compare(WWW_AUTHENTICATE) == 0)
	// 		header->wwwAuthenticate(it->getValue()); 
	// 	// if not recognized ignored and not added
	// 	it++;
	// }
	return (header);
}
// #endif
