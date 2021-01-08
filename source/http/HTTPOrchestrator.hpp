/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPOrchestrator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:34:10 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/10 10:55:26 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPORCHESTRATOR_HPP_
# define HTTPORCHESTRATOR_HPP_

#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <sys/select.h>
#include <buffer/impl/FileDescriptorBuffer.hpp>
#include <list>
#include <map>

class Configuration;
class Environment;

class Logger;

class HTTPOrchestrator
{
	public:
		typedef std::list<HTTPServer*> server_container;
		typedef server_container::iterator server_iterator;

	public:
		static Logger &LOG;

	private:
		server_container m_servers;
		fd_set m_fds;
		int m_highestFd;
		int m_fdCount;
		std::map<int, HTTPServer const*> serverFds;
		std::map<int, FileDescriptorBuffer*> fileReadFds;
		std::map<int, HTTPClient*> clientFds;
		std::map<int, FileDescriptorBuffer*> fileWriteFds;
		bool m_running;
		bool m_stopping;

	private:
		HTTPOrchestrator(void);
		HTTPOrchestrator(const server_container &servers);
		HTTPOrchestrator(const HTTPOrchestrator &other);

		HTTPOrchestrator&
		operator=(const HTTPOrchestrator &other);

	public:
		virtual
		~HTTPOrchestrator();

	private:
		void
		prepare(void);

		void
		unprepare(void);

		void
		setFd(int fd);

		void
		clearFd(int fd);

		void
		printSelectOutput(fd_set &readFds, fd_set &writeFds, bool forced = false);

		void
		addServer(HTTPServer &server);

		void
		addFileDescriptorBufferRead(FileDescriptorBuffer &fileDescriptorBuffer);

		void
		addClient(HTTPClient &client);

		void
		addFileDescriptorBufferWrite(FileDescriptorBuffer &fileDescriptorBuffer);

		void
		removeServer(int fd);

		void
		removeFileRead(int fd);

		void
		removeClient(int fd);

		void
		removeFileWrite(int fd);

	public:
		void
		start();

		void
		terminate();

	public:
		static HTTPOrchestrator*
		create();

		bool
		running() const
		{
			return (m_running);
		}

		bool
		stopping() const
		{
			return (m_stopping);
		}
};

#endif
