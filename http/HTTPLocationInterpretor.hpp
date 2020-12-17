/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPLocationInterpretor.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:11:25 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/17 16:55:41 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPLOCATIONINTERPRETOR_HPP_
# define HTTPLOCATIONINTERPRETOR_HPP_

#include <config/block/ServerBlock.hpp>
#include <list>
#include <iostream>

class HTTPLocationInterpretor
{
	public:
		enum State
		{
			S_NOT_STARTED,
			S_FIRST,
			S_START,
			S_EXACT_INTRO,
			S_EXACT,
			S_MIDDLE_START,
			S_MIDDLE,
			S_TILDE,
			S_SPACE_AFTER_TILDE,
			S_NO_CASE_MODIFIER,
			S_SPACE_AFTER_NO_CASE_MODIFIER,
			S_END,
			S_OVER,
		};
	
	private:
		State m_state;
		std::string m_path;
		std::string m_start;
		std::list<std::string> m_middleList;
		std::string m_middle;
		std::string m_end;;
		std::string m_exact;

		char m_lastChar;
		
	public:
		HTTPLocationInterpretor();
		HTTPLocationInterpretor(std::string path);
		HTTPLocationInterpretor(const HTTPLocationInterpretor &other);
		
		virtual
		~HTTPLocationInterpretor();

		HTTPLocationInterpretor&
		operator =(const HTTPLocationInterpretor &other);

		const std::string&
		start(void);

		const std::string&
		end(void);
		
		const std::string&
		exact(void);

		const char&
		lastChar(void);
		
		std::list<std::string>&
		middleList(void);

		void
		erase(std::list<std::string>::iterator it);
		
		void
		setEnd(std::string end);
		
		bool
		next(char &c);

		void
		consume(char &c);

		void
		end(std::string &end);
};

#endif /* HTTPLocationInterpretor_HPP_ */