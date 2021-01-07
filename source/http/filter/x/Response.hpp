/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:51:19 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 18:51:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP_
# define RESPONSE_HPP_

#include <http/enums/HTTPStatus.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <io/FileDescriptor.hpp>
#include <util/Optional.hpp>
#include <string>

class HTTPStatus;

class Response
{
	public:
		class IBody;

	private:
		Optional<const HTTPStatus*> m_status;
		HTTPHeaderFields m_headers;
		IBody *m_body;

	public:
		Response();
		Response(const Response &other);

		virtual
		~Response();

		Response&
		operator=(const Response &other);

		void
		status(const HTTPStatus &status);

		Optional<const HTTPStatus*>&
		status();

		HTTPHeaderFields&
		headers();

		void
		body(IBody *body, bool deletePrevious = true);

		IBody*
		body();

	public:
		class IBody
		{
			public:
				virtual
				~IBody();
		};

		class StringBody :
				public IBody
		{
			public:
				virtual
				~StringBody();

				StringBody(const std::string &content);
		};

		class FileBody :
				public IBody
		{
			public:
				virtual
				~FileBody();

				FileBody(FileDescriptor &fd);
		};

		class CGIBody :
				public IBody
		{
			public:
				virtual
				~CGIBody();
		};

};

#endif /* RESPONSE_HPP_ */
