/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:29:02 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/11 11:23:21 by alicetetu        ###   ########.fr       */
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
#include <http/HTTPHeaderParser.hpp>

class HTTPRequestParser
{
	public:
		enum State
		{
			S_NOT_STARTED,
	//		S_ERROR,
			S_METHOD,
			S_SPACES_BEFORE_PATH,
			S_PATH,
//			S_QUERY_STRING_START,
//			S_QUERY_STRING,
//			S_FRAGMENT_START,
//			S_FRAGMENT,
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
			S_FIELD,
		//	S_EMPTY_LINE,
			S_CONTINUE,
			S_END,
		};

	private:
		State m_state;
		std::string m_method;
		std::string m_path;
		int m_major;
		int m_minor;

		char m_last_char;
		char m_last_char2;

		std::vector<HTTPHeaderParser> m_headerParser;

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

		void
		header(HTTPHeaderParser headerParser);

		std::vector<HTTPHeaderParser> 
		getHeader();

		char
		lastChar() const;
};

#endif /* HTTPREQUESTPARSER_HPP_ */
