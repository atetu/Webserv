/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHTTPResponse.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 21:09:07 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/20 21:09:07 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHTTPRESPONSE_HPP_
# define CGIHTTPRESPONSE_HPP_

#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/response/HTTPResponse.hpp>

class CGIHTTPResponse :
		public HTTPResponse
{
	private:
		const HTTPStatusLine m_statusLine;
		CommonGatewayInterface &m_cgi;
		FileDescriptorBuffer *m_inBuffer;
		FileDescriptorBuffer *m_outBuffer;
		State m_state;

	private:
		CGIHTTPResponse();
		CGIHTTPResponse(const CGIHTTPResponse &other);

		CGIHTTPResponse&
		operator=(const CGIHTTPResponse &other);

	public:
		CGIHTTPResponse(const HTTPStatusLine &statusLine, CommonGatewayInterface &cgi);

		virtual
		~CGIHTTPResponse();

		virtual bool
		write(SocketBuffer &socketBuffer);

		virtual void
		readFileDescriptors(fdb_vector &out);

		virtual void
		writeFileDescriptors(fdb_vector &out);

		virtual HTTPStatusLine
		statusLine() const;

		virtual State
		state() const;
};

#endif /* CGIHTTPRESPONSE_HPP_ */