/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GenericHTTPResponse.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 16:38:46 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 10:20:36 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERICHTTPRESPONSE_HPP_
# define GENERICHTTPRESPONSE_HPP_

#include <http/header/HTTPHeaderFields.hpp>
#include <http/response/HTTPResponse.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <sys/types.h>
#include <string>

class HTTPStatus;
class FileDescriptorBuffer;
class SocketBuffer;

class GenericHTTPResponse :
		public HTTPResponse
{
	public:
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
				FileDescriptorBuffer &m_fileDescriptorBuffer;

			private:
				FileBody(void);
				FileBody(const FileBody &other);

				FileBody&
				operator =(const FileBody &other);

			public:
				FileBody(FileDescriptorBuffer &fileBuffer);

				virtual
				~FileBody();

				virtual bool
				write(SocketBuffer &fd);

				FileDescriptorBuffer&
				fileDescriptorBuffer();

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

	private:
		HTTPStatusLine m_statusLine;
		HTTPHeaderFields m_headers;
		IBody *m_body;
		State m_state;
		ssize_t m_state_index;

	private:
		GenericHTTPResponse();
		GenericHTTPResponse(const GenericHTTPResponse &other);

		GenericHTTPResponse&
		operator=(const GenericHTTPResponse &other);

	public:
		GenericHTTPResponse(const HTTPStatusLine statusLine, const HTTPHeaderFields &headers, IBody *body);

		virtual
		~GenericHTTPResponse();

		bool
		write(SocketBuffer &socketBuffer);

		IBody*
		body() const
		{
			return (m_body);
		}

		void
		readFileDescriptors(fdb_vector &out);

		void
		writeFileDescriptors(fdb_vector &out);

		HTTPStatusLine
		statusLine() const;

		State
		state() const;

	public:
		static GenericHTTPResponse*
		status(HTTPStatus &status, const HTTPHeaderFields &headers = HTTPHeaderFields());

		static GenericHTTPResponse*
		file(HTTPStatus &status, const HTTPHeaderFields &headers, FileDescriptorBuffer &fileBuffer);

		static GenericHTTPResponse*
		string(HTTPStatus &status, const HTTPHeaderFields &headers, const std::string &string);
};

#endif /* GENERICHTTPRESPONSE_HPP_ */
