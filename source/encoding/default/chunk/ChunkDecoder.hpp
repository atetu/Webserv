/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:47:44 by alicetetu         #+#    #+#             */
/*   Updated: 2021/01/07 14:32:35 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNKDECODER_HPP_
# define CHUNKDECODER_HPP_

#include <string>

class ChunkDecoder
{
	public:

		enum State
		{
			S_NOT_STARTED,
			S_SIZE,
		//	S_EXTENSION,
		//	S_SIZE_END,
		//	S_SIZE_END2,
			S_CHUNK,
		//	S_CHUNK_END_EXPECTED,
			S_CHUNK_END,
		//	S_CHUNK_END2,
			S_NULL,
		//	S_END,
			S_OVER
		};

		ChunkDecoder();
		ChunkDecoder(const std::string &input);
		// ChunkDecoder(std::string &body);
		ChunkDecoder(const ChunkDecoder &other);

		virtual
		~ChunkDecoder();

		ChunkDecoder&
		operator=(const ChunkDecoder &other);

		std::string
		decode(std::string storage);
		
		ChunkDecoder::State state();

		static inline bool // why inline?
		isValidCharacter(char c)
		{
			return (std::isalnum(c));
		}

	private:
		std::string m_input;
		State m_state;
		int m_sizeNb;
		std::string m_sizeStr;
		std ::string m_parsedData;
		std::string m_parsedChunk;
		std::string m_extension;
		char m_lastChar;
		size_t m_totalSize;
};

#endif /* CHUNKDECODER_HPP_ */
