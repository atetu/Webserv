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

#include <http/parser/HTTPHeaderFieldsParser.hpp>
#include <http/parser/HTTPRequestPathParser.hpp>
#include <string>

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
			S_HEADER_FIELDS,
			S_END,
		};

	private:
		State m_state;
		std::string m_method;
		HTTPRequestPathParser m_pathParser;
		int m_major;
		int m_minor;
		HTTPHeaderFieldsParser m_headerFieldsParser;
		std::string m_body;
		char m_last;
		char m_last2;

	public:
		HTTPRequestParser();

		void
		consume(char c);

		State
		state() const;

		std::string
		method() const;

		inline const HTTPRequestPathParser&
		pathParser() const
		{
			return (m_pathParser);
		}

		inline int
		major() const
		{
			return (m_major);
		}

		inline int
		minor() const
		{
			return (m_major);
		}

		inline const HTTPHeaderFieldsParser&
		headerFieldsParser() const
		{
			return (m_headerFieldsParser);
		}

		inline const HTTPHeaderFields&
		headerFields() const
		{
			return (m_headerFieldsParser.headerFields());
		}

		void
		body(const std::string &storage);

		const std::string&
		body() const;

		void
		body(std::string &storage);

		URL
		url();
};

#endif /* HTTPREQUESTPARSER_HPP_ */