/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 16:38:46 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/27 16:38:46 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SRC_HTTP_HTTPRESPONSE_HPP_
# define SRC_HTTP_HTTPRESPONSE_HPP_

#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <sys/types.h>
#include <util/buffer/IOBuffer.hpp>
#include <string>

# define AWAITING_BUFFER_SIZE 512

class HTTPResponse
{
	public:
		class StatusLine
		{
			private:
				HTTPVersion m_version;
				HTTPStatus m_status;

			public:
				StatusLine(void);
				StatusLine(const HTTPStatus &status);
				StatusLine(const HTTPVersion &version, const HTTPStatus &status);
				StatusLine(const StatusLine &other);

				virtual
				~StatusLine();

				StatusLine&
				operator=(const StatusLine &other);

				std::string
				format(void) const;
		};

		class IBody
		{
			public:
				virtual
				~IBody();

				virtual bool
				write(IOBuffer &fd) = 0;
		};

		class FileBody :
				public IBody
		{
			private:
				IOBuffer m_fd;

			public:
				FileBody(int fd);

				virtual
				~FileBody();

				virtual bool
				write(IOBuffer &fd);
		};

		class StringBody :
				public IBody
		{
			private:
				std::string m_string;
				bool m_sent;

			public:
				StringBody(std::string string);

				virtual
				~StringBody();

				virtual bool
				write(IOBuffer &fd);
		};

		enum State
		{
			NONE = 0,
			HEADERS,
			BODY,
			FLUSHING,
			FINISHED,
		};

	private:
		StatusLine m_statusLine;
		HTTPHeaderFields m_headers;
		IBody *m_body;
		State m_state;
		ssize_t m_state_index;

	public:
		HTTPResponse(const HTTPVersion &version, const HTTPStatus &status, const HTTPHeaderFields &headers, IBody *body);

		virtual
		~HTTPResponse();

		bool
		write(IOBuffer &fd);

	public:
		static inline HTTPResponse*
		status(HTTPStatus &status)
		{
			return (new HTTPResponse(HTTPVersion::HTTP_1_1, status, HTTPHeaderFields(), NULL));
		}
};

#endif /* SRC_HTTP_HTTPRESPONSE_HPP_ */
