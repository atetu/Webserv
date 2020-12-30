/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkEncoder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 14:47:44 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/21 15:47:41 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNKENCODER_HPP_
# define CHUNKENCODER_HPP_

#include <string>

class ChunkEncoder 
{
	public:

		enum State
		{
			S_NOT_STARTED,
			S_SIZE,
			S_EXTENSION,
			S_SIZE_END,
			S_SIZE_END2,
			S_CHUNK,
			S_CHUNK_END_EXPECTED,
			S_CHUNK_END,
			S_NULL,
			S_END,
			S_OVER
		};

		ChunkEncoder();
		ChunkEncoder(const std::string &input);
		ChunkEncoder(const ChunkEncoder &other);

		virtual
		~ChunkEncoder();

		ChunkEncoder&
		operator=(const ChunkEncoder &other);

		std::string
		encode();

		static inline bool // why inline?
		isValidCharacter(char c)
		{
			return (std::isalnum(c));
		}

	private:
		std::string m_input;
		std::string m_parsedData;
};

#endif /* CHUNKENCODER_HPP_ */
