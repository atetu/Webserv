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
		bool m_closeOnDestroy;
		size_type m_maxSize;
		bool m_readEverything;

	public:
		IOBuffer(void);
		IOBuffer(int fd, bool closeOnDestroy = true, size_type maxSize = std::string::npos);
		IOBuffer(const IOBuffer &other);

		virtual
		~IOBuffer();

		IOBuffer&
		operator =(const IOBuffer &other);

		inline
		operator int(void) const
		{
			return (m_fd);
		}

		void
		storeFrom(IOBuffer &buffer, bool andClear = true)
		{
			size_type capacity = std::min(this->capacity(), buffer.capacity());

			if (capacity)
			{
				m_storage += buffer.storage().substr(0, capacity);

				buffer.storage().erase(0, capacity);
			}

			if (andClear)
				buffer.clear();
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
		capacity() const;

		void
		close(void);

		inline bool
		hasReadEverything() const
		{
			return (m_readEverything);
		}

		inline int
		fd() const
		{
			return (m_fd);
		}
};

#endif /* IOBUFFER_HPP_ */
