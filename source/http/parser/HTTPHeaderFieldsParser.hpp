/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPHeaderFieldsParser.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 21:46:29 by ecaceres          #+#    #+#             */
/*   Updated: 2020/01/01 21:46:29 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERFIELDSPARSER_HPP_
# define HTTPHEADERFIELDSPARSER_HPP_

#include <http/header/HTTPHeaderFields.hpp>
#include <string>

class HTTPHeaderFieldsParser
{
	public:
		enum State
		{
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
		HTTPHeaderFields m_headerFields;
		std::string m_key;
		std::string m_value;
		char m_last;
		char m_last2;

	public:
		HTTPHeaderFieldsParser();

		void
		consume(char c);

		inline State
		state() const
		{
			return (m_state);
		}

		inline const HTTPHeaderFields&
		headerFields() const
		{
			return (m_headerFields);
		}

	private:
		void
		commit(State nextState);
};

#endif /* HTTPHEADERFIELDSPARSER_HPP_ */
