/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IBuffer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 18:19:03 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/29 18:19:03 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IBUFFER_HPP_
# define IBUFFER_HPP_

#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

class IBuffer
{
	public:
		virtual
		~IBuffer(void)
		{
		}

		virtual std::string&
		storage() = 0;

		virtual const std::string&
		storage() const = 0;
};

#endif /* IBUFFER_HPP_ */
