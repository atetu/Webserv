/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:29:02 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/23 16:05:04 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER_HPP_
# define HTTPREQUESTPARSER_HPP_

#include <http/header/HTTPHeaderFields.hpp>
#include <http/header/HTTPHeaderParser.hpp>
#include <map>
#include <string>
#include <vector>

class URL;

# ifdef major
#  undef major
# endif

# ifdef minor
#  undef minor
# endif

class HTTPRequestParser
{
	public:
		enum State
		{
			S_NOT_STARTED = 0,
			S_METHOD,
			S_SPACES_BEFORE_PATH,
			S_PATH,
			S_HTTP_START,
			S_HTTP_H,
			S_HTTP_HT,
			S_HTTP_HTT,
			S_HTTP_HTTP,
			S_HTTP_SLASH,
			S_HTTP_MAJOR,
			S_HTTP_DOT,
			S_HTTP_MINOR,
			S_HTTP_END,
			S_HTTP_END2,
			S_HTTP_END3,
			S_QUERY_STRING_KEY,
			S_QUERY_STRING_VALUE,
			S_FRAGMENT,
			S_HEX_START,
			S_HEX_END,
			S_FIELD,
			S_COLON,
			S_SPACES_BEFORE_VALUE,
			S_VALUE,
			S_SPACES_AFTER_VALUE,
			S_VALUE_END,
			S_VALUE_END2,
			S_VALUE_END3,
			S_END,
		};

	private:
		State m_state;
		std::string m_method;
		std::string m_path;
		int m_major;
		int m_minor;
		std::string m_field;
		std::string m_value;
		HTTPHeaderFields m_headerFields;
		std::map<std::string, std::string> m_query;
		std::string m_queryKey;
		std::string m_queryValue;
		std::string m_fragment;
		std::string m_body;
		State m_hexBeforeState;
		std::string *m_hexStoreTarget;
		std::string m_hex;

		char m_last_char;
		char m_last_char2;

	public:
		HTTPRequestParser();

		void
		consume(char c);

		State
		state() const;

		std::string
		method() const;

		std::string
		path() const;

		int
		major() const;

		int
		minor() const;

		const HTTPHeaderFields&
		headerFields();

		void
		header(HTTPHeaderParser headerParser);

		std::vector<HTTPHeaderParser>
		getHeader();

		char
		lastChar() const;

		std::map<std::string, std::string>&
		query();

		std::string&
		fragment();

		void
		body(const std::string &storage);

		const std::string&
		body() const;

		void
		body(std::string &storage);

		URL
		url();

	private:
		inline void
		hexStart(std::string *storeInto)
		{
			m_hexBeforeState = m_state;
			m_state = S_HEX_START;
			m_hexStoreTarget = storeInto;
		}
};

#endif /* HTTPREQUESTPARSER_HPP_ */
