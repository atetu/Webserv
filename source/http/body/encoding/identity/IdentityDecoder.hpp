/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IdentityDecoder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 01:26:21 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/09 01:26:21 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDENTITYDECODER_HPP_
# define IDENTITYDECODER_HPP_

#include <http/body/encoding/IHTTPBodyDecoder.hpp>
#include <util/Singleton.hpp>

class IdentityDecoder :
		public IHTTPBodyDecoder,
		public Singleton<IdentityDecoder>
{
	private:
		IdentityDecoder(const IdentityDecoder &other);

		IdentityDecoder&
		operator=(const IdentityDecoder &other);

	public:
		IdentityDecoder();

		virtual
		~IdentityDecoder();

		bool
		consume(std::string &out, char c);
};

#endif /* IDENTITYDECODER_HPP_ */
