/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:26:37 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:26:37 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <encoding/default/base64/Base64.hpp>

const std::string Base64::NAME = "base64";
const std::string Base64::CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

Base64Encoder Base64::ENCODER;
Base64Decoder Base64::DECODER;

Base64::~Base64()
{
}

void
Base64::load()
{
	EncodingRegistry::get().add(NAME, ENCODER);
	EncodingRegistry::get().add(NAME, DECODER);
}
