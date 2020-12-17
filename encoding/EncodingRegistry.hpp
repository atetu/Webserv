/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EncodingRegistry.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:13:17 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:13:17 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENCODINGREGISTRY_HPP_
# define ENCODINGREGISTRY_HPP_

#include <encoding/Decoder.hpp>
#include <encoding/Encoder.hpp>
#include <map>
#include <string>

class Logger;

class EncodingRegistry
{
	public:
		typedef std::map<std::string, Encoder*> encoder_map;
		typedef std::map<std::string, Decoder*> decoder_map;

	public:
		static Logger &LOG;

	private:
		encoder_map m_encoders;
		decoder_map m_decoders;

	private:
		EncodingRegistry();
		EncodingRegistry(const EncodingRegistry &other);

		EncodingRegistry&
		operator=(const EncodingRegistry &other);

	public:
		virtual
		~EncodingRegistry();

		bool
		add(const std::string &name, Encoder &encoder);

		bool
		add(const std::string &name, Decoder &decoder);

	public:
		static EncodingRegistry&
		get(void);
};

#endif /* ENCODINGREGISTRY_HPP_ */
