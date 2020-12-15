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

		/**
		 * Base body class.
		 */
		class IBody
		{
			public:
				virtual
				~IBody();

				/**
				 * Write to the output information that will be sent to the socket.
				 *
				 * @param buffer IO Buffer to write to.
				 * @return Whether or not everything has been sent.
				 */
				virtual bool
				write(IOBuffer &buffer) = 0;

				virtual bool
				isDone() = 0;
		};

		/**
		 * The File Body fill-up an internal buffer with information of a source IOBuffer.
		 * Slowly reading (from a file) then writing (to the output) to return the file in small chunks.
		 */
		class FileBody :
				public IBody
		{
			private:
				IOBuffer m_buffer;

			public:
				FileBody(int fd);

				virtual
				~FileBody();

				virtual bool
				write(IOBuffer &fd);

				IOBuffer&
				buffer();

				bool
				isDone();
		};

		/**
		 * The String Body only put the whole provided string into the IOBuffer.
		 */
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

				bool
				isDone();
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
		HTTPResponse(const HTTPStatus &status, const HTTPHeaderFields &headers, IBody *body);
		HTTPResponse(const HTTPVersion &version, const HTTPStatus &status, const HTTPHeaderFields &headers, IBody *body);

		virtual
		~HTTPResponse();

		bool
		write(IOBuffer &fd);

		IBody*
		body() const
		{
			return (m_body);
		}

		State
		state() const
		{
			return (m_state);
		}

	public:
		static inline HTTPResponse*
		status(HTTPStatus &status)
		{
			return (new HTTPResponse(HTTPVersion::HTTP_1_1, status, HTTPHeaderFields(), NULL));
		}
};

#endif /* SRC_HTTP_HTTPRESPONSE_HPP_ */
