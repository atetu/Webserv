/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketBuffer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 22:38:35 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 22:38:35 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETBUFFER_HPP_
# define SOCKETBUFFER_HPP_

#include <io/Socket.hpp>
#include <stddef.h>
#include <sys/types.h>
#include <util/buffer/impl/FileBuffer.hpp>
#include <string>

class SocketBuffer :
		public FileBuffer
{
	private:
		SocketBuffer(void);
		SocketBuffer(const SocketBuffer &other);

		SocketBuffer&
		operator =(const SocketBuffer &other);

	protected:
		SocketBuffer(Socket &socket, int actionOnDestroy, size_type maxSize);

	public:
		virtual
		~SocketBuffer();

		ssize_t
		recv(size_t len = 2048);

		ssize_t
		send(size_t len = 2048);

	public:
		static SocketBuffer*
		from(Socket &socket, int actionOnDestroy = FileBuffer::CLOSE, size_type maxSize = std::string::npos);
};

#endif /* SOCKETBUFFER_HPP_ */
