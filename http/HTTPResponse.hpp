/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 16:38:46 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/09 15:51:48 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP_
# define HTTPRESPONSE_HPP_

#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <sys/types.h>
#include <string>

class FileBuffer;
class SocketBuffer;

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
				 * @param buffer IO Buffer to write to. // TODO
				 * @return Whether or not everything has been sent.
				 */
				virtual bool
				write(SocketBuffer &socketBuffer) = 0;

				virtual bool
				isDone() = 0;
		};

		/**
		 * The File Body fill-up an internal buffer with information of a source IOBuffer. // TODO
		 * Slowly reading (from a file) then writing (to the output) to return the file in small chunks.
		 */
		class FileBody :
				public IBody
		{
			private:
				FileBuffer &m_fileBuffer;

			private:
				FileBody(void);
				FileBody(const FileBody &other);

				FileBody&
				operator =(const FileBody &other);

			public:
				FileBody(FileBuffer &fileBuffer);

				virtual
				~FileBody();

				virtual bool
				write(SocketBuffer &fd);

				FileBuffer&
				fileBuffer();

				bool
				isDone();
		};

		/**
		 * The String Body only put the whole provided string into the IOBuffer. // TODO
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
				write(SocketBuffer &socketBuffer);

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
		write(SocketBuffer &socketBuffer);

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

#endif /* HTTPRESPONSE_HPP_ */
