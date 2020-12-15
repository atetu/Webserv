/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeaderParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 11:39:52 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/10 17:41:25 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERPARSER_HPP_
# define HTTPHEADERPARSER_HPP_

# include <iostream>

class HTTPHeaderParser
{
	public:
		enum State
		{
			S_NOT_STARTED,
	//		S_ERROR,
			S_COLON,
			S_FIELD,
			S_SPACES_BEFORE_VALUE,
			S_VALUE,
			S_SPACES_AFTER_VALUE,
			S_VALUE_END,
			S_VALUE_END2,
			S_HEADER,
			S_CONTINUE,
		//	S_EMPTY_LINE,
			S_END,
		};

	private:
		State m_state;
		std::string m_field;
		std::string m_value;
		
		char m_last_char;
		char m_last_char2;

	public:
		HTTPHeaderParser();

		void
		consume(char c);

		State
		state() const;

		void
		header(HTTPHeaderParser headerParser);

		std::string&
		getField();

		std::string&
		getValue();
		
};

#endif /* HTTPHEADERPARSER_HPP_ */
