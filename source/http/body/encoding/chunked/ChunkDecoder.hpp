/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:47:44 by alicetetu         #+#    #+#             */
/*   Updated: 2021/01/21 19:45:20 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNKDECODER_HPP_
# define CHUNKDECODER_HPP_

#include <http/body/encoding/IHTTPBodyDecoder.hpp>
#include <stddef.h>
#include <string>

class ChunkDecoder :
		public IHTTPBodyDecoder
{
	public:

	enum State
		{
			S_NOT_STARTED = 0,
			S_SIZE,
		//	S_EXTENSION,
		//	S_SIZE_END,
		//	S_SIZE_END2,
			S_CHUNK,
		//	S_CHUNK_END_EXPECTED,
			S_CHUNK_END,
			S_CHUNK_END2,
		//	S_NULL_END,
		//	S_END,
			S_OVER
		};

	private:
		bool m_isAllocated;
		State m_state;

	private:
		ChunkDecoder();
		ChunkDecoder(const ChunkDecoder &other);

		ChunkDecoder&
		operator=(const ChunkDecoder &other);

	public:
		ChunkDecoder(bool isAllocated);

		virtual
		~ChunkDecoder();

		bool
		consume(const std::string &in, std::string &out, size_t &consumed, bool max);

		void
		cleanup();

		ChunkDecoder::State
		state();

		static inline bool 
		isValidCharacter(char c)
		{
			return (std::isalnum(c));
		}

	private:
		int m_sizeNb;
		std::string m_sizeStr;
		std::string m_parsedChunk;
		long long m_totalSize;
};

#endif /* CHUNKDECODER_HPP_ */
