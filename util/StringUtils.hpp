/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 20:48:42 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/21 20:48:42 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGUTILS_HPP_
# define STRINGUTILS_HPP_

#include <string>

class StringUtils
{
	private:
		StringUtils();
		StringUtils(const StringUtils &other);

	public:
		virtual
		~StringUtils();

	private:
		StringUtils&
		operator=(const StringUtils &other);

	public:
		static std::string
		toUpperCase(const std::string &string);

		static std::string
		replace(const std::string &in, char c, char to);

		static bool
		equalsIgnoreCase(const std::string &a, const std::string &b);
};

#endif /* STRINGUTILS_HPP_ */
