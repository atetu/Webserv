/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeaderFields.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 18:39:24 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 12:44:58 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/header/HTTPHeaderFields.hpp>
#include <http/mime/Mime.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <util/Convert.hpp>
#include <util/Enum.hpp>
#include <util/URL.hpp>

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
const std::string HTTPHeaderFields::MIME_HTTP/*	        */= "message/http";

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
HTTPHeaderFields::location(const URL &url)
{
	return (location(url.format()));
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
HTTPHeaderFields::wwwAuthenticate(const std::string &type)
{
	return (set(WWW_AUTHENTICATE, type));
}

HTTPHeaderFields&
HTTPHeaderFields::wwwAuthenticate(const std::string &type, const std::string &realm)
{
	return (set(WWW_AUTHENTICATE, type + " realm=\"" + realm + "\""));
}

HTTPHeaderFields&
HTTPHeaderFields::wwwAuthenticate(const std::string &type, const Optional<std::string> realm)
{
	if (realm.present())
		return (wwwAuthenticate(type, realm.get()));

	return (wwwAuthenticate(type));
}

HTTPHeaderFields&
HTTPHeaderFields::html(void)
{
	return (contentType(MIME_HTML));
}

HTTPHeaderFields&
HTTPHeaderFields::httpMessage(void)
{
	return (contentType(MIME_HTTP));
}

HTTPHeaderFields&
HTTPHeaderFields::set(const std::string &key, const std::string &value)
{
	m_storage[key] = value;

	return (*this);
}

const Optional<std::string>
HTTPHeaderFields::get(const std::string &key) const
{
	const_iterator it = m_storage.find(key);

	if (it == m_storage.end())
		return (Optional<std::string>());

	return (Optional<std::string>(it->second));
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
HTTPHeaderFields::format(const std::string &separator) const
{
	static std::string dotAndASpace = ": ";

	std::string str;

	for (const_iterator it = begin(); it != end(); it++)
		str += it->first + dotAndASpace + it->second + separator;

	return (str);
}

bool
HTTPHeaderFields::empty(void)
{
	return (m_storage.empty());
}

std::map<std::string, std::string>&
HTTPHeaderFields::storage(void)
{
	return (m_storage);
}

const std::map<std::string, std::string>&
HTTPHeaderFields::storage(void) const
{
	return (m_storage);
}