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
		public IHTTPBodyDecoder
{
	private:
		bool m_isAllocated;
		long long m_contentLength;
		long long m_consumed;

	private:
		IdentityDecoder();
		IdentityDecoder(const IdentityDecoder &other);

		IdentityDecoder&
		operator=(const IdentityDecoder &other);

	public:
		IdentityDecoder(bool isAllocated, long long contentLength);

		virtual
		~IdentityDecoder();

		bool
		consume(std::string &out, char c);

		void
		cleanup();
};

#endif /* IDENTITYDECODER_HPP_ */
