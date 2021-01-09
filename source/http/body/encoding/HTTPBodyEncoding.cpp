/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPBodyEncoding.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 01:32:18 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/09 01:32:18 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/body/encoding/chunked/ChunkDecoder.hpp>
#include <http/body/encoding/exception/HTTPBodyEncodingException.hpp>
#include <http/body/encoding/HTTPBodyEncoding.hpp>
#include <http/body/encoding/identity/IdentityDecoder.hpp>
#include <util/Number.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <string>

HTTPBodyEncoding::~HTTPBodyEncoding()
{
}

IHTTPBodyDecoder&
HTTPBodyEncoding::decoderFor(const HTTPHeaderFields &headerFields)
{
	Optional<std::string> transfertEncodingOpt = headerFields.get(HTTPHeaderFields::TRANSFER_ENCODING);
	if (transfertEncodingOpt.present())
	{
		const std::string &transfertEncoding = transfertEncodingOpt.get();

		if (transfertEncoding == HTTPHeaderFields::CHUNKED)
			return (*(new ChunkDecoder(true)));

		throw HTTPBodyEncodingException("unsupported transfert-encoding:" + transfertEncoding);
	}

	Optional<std::string> contentLengthOpt = headerFields.get(HTTPHeaderFields::CONTENT_TYPE);
	if (contentLengthOpt.present())
	{
		try
		{
			Number::parse<long>(contentLengthOpt.get());

			return (IdentityDecoder::instance());
		}
		catch (Exception &exception)
		{
			throw HTTPBodyEncodingException("content-length is not a valid number: " + exception.message());
		}
	}

	throw HTTPBodyEncodingException("no transfert-encoding or content-length");
}
