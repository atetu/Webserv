/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPLocationInterpretor.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:11:25 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/15 17:29:39 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPLOCATIONINTERPRETOR_HPP_
# define HTTPLOCATIONINTERPRETOR_HPP_

#include <config/block/CGIBlock.hpp>
#include <config/block/ServerBlock.hpp>

class HTTPLocationInterpretor
{
	public:
		enum State
		{
			S_NOT_STARTED,
			S_START,
			S_END,
		};
	
	private:
		State m_state;
		std::string m_path;
		std::string m_start;
		std::string m_middle;
		std::string m_end;;
		
	public:
		HTTPLocationInterpretor();
		HTTPLocationInterpretor(std::string path);
		HTTPLocationInterpretor(const HTTPLocationInterpretor &other);
		
		virtual
		~HTTPLocationInterpretor();

		HTTPLocationInterpretor&
		operator =(const HTTPLocationInterpretor &other);

		std::string
		start(void);

		std::string
		middle(void);

		std::string
		end(void);
		
		bool
		next(char &c);

		void
		consume(char &c);
};

#endif /* HTTPLocationInterpretor_HPP_ */