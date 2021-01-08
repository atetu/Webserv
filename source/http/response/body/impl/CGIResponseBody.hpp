/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseBody.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 17:24:51 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 17:24:51 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIRESPONSEBODY_HPP_
# define CGIRESPONSEBODY_HPP_

#include <buffer/impl/BaseBuffer.hpp>
#include <http/response/body/IResponseBody.hpp>
#include <nio/NIOSelector.hpp>

class CommonGatewayInterface;

class CGIResponseBody :
		public IResponseBody,
		public NIOSelector::Callback
{
	private:
		CommonGatewayInterface &m_cgi;
		FileDescriptorBuffer &m_bufferedIn;
		FileDescriptorBuffer &m_bufferedOut;

	private:
		CGIResponseBody();
		CGIResponseBody(const CGIResponseBody &other);

		CGIResponseBody&
		operator=(const CGIResponseBody &other);

	public:
		CGIResponseBody(CommonGatewayInterface &cgi);

		virtual
		~CGIResponseBody();

		bool
		isSelfManaged() const;

		bool
		store(BaseBuffer &buffer);

		bool
		writable(FileDescriptor &fd);

		bool
		readable(FileDescriptor &fd);

		bool
		isDone();
};

#endif /* CGIRESPONSEBODY_HPP_ */