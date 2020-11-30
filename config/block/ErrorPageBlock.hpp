/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageBlock.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 14:26:30 by alicetetu         #+#    #+#             */
/*   Updated: 2020/11/30 15:40:40 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGEBLOCK_HPP_
# define ERRORPAGEBLOCK_HPP_

#include <util/Optional.hpp>
#include <string>

class ErrorPageBlock
{
	private:
		std::string m_path;
		Optional<int> m_errorCode;

	public:
		ErrorPageBlock(void);
		ErrorPageBlock(const ErrorPageBlock &other);

		virtual
		~ErrorPageBlock();

		ErrorPageBlock&
		operator=(const ErrorPageBlock &other);

		ErrorPageBlock&
		path(std::string path);
		
		ErrorPageBlock&
		code(int code);
};

#endif
