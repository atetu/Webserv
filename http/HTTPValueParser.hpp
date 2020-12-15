/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPValueParser.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 16:10:13 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/14 13:30:01 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTPValueParser_HPP_
# define HTTPValueParser_HPP_

# include <iostream>
# include <vector>
# include <map>
# include <list>
#include <http/HTTPHeaderParser.hpp>

class HTTPValueParser
{
	public :
		typedef std::map<float, std::list<std::string> > map;
	
	public:
		enum State
		{
			S_NOT_STARTED,
			S_VALUE,
		//	S_SPACES_BEFORE_VALUE,
			S_SPACES_AFTER_VALUE,
			S_SEMI_COLON,
			S_WEIGHT_START1,
			S_WEIGHT_START2,
			S_SPACES_BEFORE_WEIGHT,
			S_WEIGHT,
			S_SPACES_AFTER_WEIGHT,
			S_END,
			// S_END2,
		};

	private:
		State m_state;
		std::string m_globalValue;
		std::string m_valueElement;
		float m_weightNb;
		std::string m_weightStr;
		map *m_map;
		
	public:
		HTTPValueParser();
		HTTPValueParser(std::string globalValue);
		~HTTPValueParser();
		
			
		void
		consume(char c);

		State
		state() const;

		map*
		getMap() const;

		 bool
		next(char &c);
};

#endif /* HTTPValueParser_HPP_ */
