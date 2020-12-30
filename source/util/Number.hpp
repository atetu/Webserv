/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Number.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 22:43:53 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/30 22:43:53 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMBER_HPP_
# define NUMBER_HPP_

#include <exception/IllegalArgumentException.hpp>
#include <util/Convert.hpp>
#include <util/StringUtils.hpp>
#include <iterator>
#include <limits>
#include <string>

class Number
{
	public:
		static const std::string BIN;
		static const std::string DECIMAL;
		static const std::string HEX;

	private:
		Number();
		Number(const Number &other);

		virtual
		~Number();

		Number&
		operator=(const Number &other);

	public:
		template<typename T>
			static T
			parse(const std::string &text, const std::string &alphabet = DECIMAL)
			{
				if (alphabet.length() <= 1)
					throw IllegalArgumentException("alphabet's length <= 1");

				std::string upper = StringUtils::toUpperCase(text);

				std::string::iterator it = upper.begin();

				int negativity = 1;
				if (*it == '-')
				{
					negativity = -1;
					it++;
				}

				long long out = 0;
				for (; it != upper.end(); it++)
				{
					char c = *it;

					std::string::size_type pos = alphabet.find(*it);
					if (pos == std::string::npos)
						throw IllegalArgumentException(std::string("char ") + *it + " is not in the alphabet: " + alphabet);

					out *= alphabet.size();
					out += pos;
				}

				out *= negativity;

				T min = std::numeric_limits<T>::min();
				T max = std::numeric_limits<T>::max();

				if (out < min)
					throw IllegalArgumentException(Convert::toString(out) + " < " + Convert::toString(min));

				if (out > max)
					throw IllegalArgumentException(Convert::toString(out) + " > " + Convert::toString(max));

				return (T(out));
			}
};

#endif /* NUMBER_HPP_ */
