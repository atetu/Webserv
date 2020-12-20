/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:29:02 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 17:06:05 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER_HPP_
# define HTTPREQUESTPARSER_HPP_

# include <string>

# ifdef major
#  undef major
# endif

# ifdef minor
#  undef minor
# endif
# include <iostream>
# include <vector>
# include <map>
#include <http/HTTPHeaderParser.hpp>

class HTTPRequestParser
{
	public:
		enum State
		{
			S_NOT_STARTED,
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
		std::map<std::string, std::string> m_headerMap;

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

		const std::map<std::string, std::string>&
		header();

		void
		header(HTTPHeaderParser headerParser);

		std::vector<HTTPHeaderParser>
		getHeader();

		char
		lastChar() const;
};

#endif /* HTTPREQUESTPARSER_HPP_ */
