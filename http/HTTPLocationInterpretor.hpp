/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPLocationInterpretor.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:11:25 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/16 16:07:53 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPLOCATIONINTERPRETOR_HPP_
# define HTTPLOCATIONINTERPRETOR_HPP_

#include <config/block/ServerBlock.hpp>


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
		std::string m_middle;
		std::string m_end;;
		std::string m_exact;
		
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
		
		bool
		next(char &c);

		void
		consume(char &c);
};

#endif /* HTTPLocationInterpretor_HPP_ */