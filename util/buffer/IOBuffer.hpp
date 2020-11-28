/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOBuffer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 22:38:35 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 22:38:35 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOBUFFER_HPP_
# define IOBUFFER_HPP_

#include <util/buffer/BaseBuffer.hpp>
#include <stdlib.h>

class IOBuffer :
		public BaseBuffer
{
	private:
		int m_fd;
		size_t m_maxSize;

	public:
		IOBuffer(void);
		IOBuffer(int fd, size_t maxSize = std::string::npos);
		IOBuffer(const IOBuffer &other);

		virtual
		~IOBuffer();

		IOBuffer&
		operator =(const IOBuffer &other);

		inline operator int(void) const
		{
			return (m_fd);
		}

		ssize_t
		read(size_t len = 2048);

		ssize_t
		recv(size_t len = 2048);

		ssize_t
		write(size_t len = 2048);

		ssize_t
		send(size_t len = 2048);

		size_t
		capacity();
};

#endif /* IOBUFFER_HPP_ */
