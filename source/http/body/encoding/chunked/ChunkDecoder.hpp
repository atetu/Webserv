/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:47:44 by alicetetu         #+#    #+#             */
/*   Updated: 2021/01/10 10:50:49 by alicetetu        ###   ########.fr       */
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
			S_NOT_STARTED,
			S_SIZE,
			S_EXTENSION,
			S_SIZE_END,
		//	S_SIZE_END2,
			S_CHUNK,
		//	S_CHUNK_END_EXPECTED,
			S_CHUNK_END,
			S_CHUNK_END2,
			S_NULL,
			S_END,
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
		consume(std::string &out, char c);

		std::string
		decode(const std::string &input);

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
		std::string m_parsedData;
		std::string m_parsedChunk;
		std::string m_extension;
		char m_lastChar;
		size_t m_totalSize;
};

#endif /* CHUNKDECODER_HPP_ */
