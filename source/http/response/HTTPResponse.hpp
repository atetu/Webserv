/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 19:46:03 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/20 19:46:03 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP_
# define HTTPRESPONSE_HPP_

#include <util/buffer/impl/FileDescriptorBuffer.hpp>
#include <vector>

class SocketBuffer;
class HTTPStatusLine;

class HTTPResponse
{
	public:
		typedef std::vector<FileDescriptorBuffer*> fdb_vector;
		typedef fdb_vector::iterator fdb_iterator;
		typedef fdb_vector::const_iterator fdb_const_iterator;

		enum State
		{
			NONE = 0,
			CONNECTING,
			STATUS_LINE,
			HEADERS,
			BODY,
			FLUSHING,
			FINISHED,
		};

	public:
		virtual
		~HTTPResponse();

		virtual bool
		write(SocketBuffer &socketBuffer) = 0;

		virtual void
		readFileDescriptors(fdb_vector &out) = 0;

		virtual void
		writeFileDescriptors(fdb_vector &out) = 0;

		virtual HTTPStatusLine
		statusLine() const = 0;

		virtual State
		state() const = 0;
};

#endif /* HTTPRESPONSE_HPP_ */
