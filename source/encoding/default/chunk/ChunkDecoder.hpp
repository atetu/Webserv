/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkDecoder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:47:44 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/23 16:57:33 by alicetetu        ###   ########.fr       */
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

		ChunkDecoder();
		ChunkDecoder(const std::string &input);
		ChunkDecoder(const ChunkDecoder &other);

		virtual
		~ChunkDecoder();

		ChunkDecoder&
		operator=(const ChunkDecoder &other);

		std::string
		decode();

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
};

#endif /* CHUNKDECODER_HPP_ */
