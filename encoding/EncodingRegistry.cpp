/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EncodingRegistry.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 23:13:17 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 23:13:17 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <encoding/EncodingRegistry.hpp>
#include <util/log/LoggerFactory.hpp>

class LoggerFactory;

Logger &EncodingRegistry::LOG = LoggerFactory::get("Encoding Registry");

EncodingRegistry::EncodingRegistry()
{
}

EncodingRegistry::~EncodingRegistry()
{
	m_encoders.clear();
	m_decoders.clear();
}

bool
EncodingRegistry::add(const std::string &name, Encoder &encoder)
{
	if (m_encoders.find(name) != m_encoders.end())
	{
		LOG.warn() << "Refused duplicate encoder: " << name << std::endl;
		return (false);
	}

	m_encoders[name] = &encoder;

	LOG.debug() << "Registered encoder: " << name << std::endl;

	return (true);
}

bool
EncodingRegistry::add(const std::string &name, Decoder &decoder)
{
	if (m_decoders.find(name) != m_decoders.end())
	{
		LOG.warn() << "Refused duplicate decoder: " << name << std::endl;
		return (false);
	}

	m_decoders[name] = &decoder;

	LOG.debug() << "Registered decoder: " << name << std::endl;

	return (true);
}

EncodingRegistry&
EncodingRegistry::get(void)
{
	static EncodingRegistry singleton;

	return (singleton);
}
