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

#include <config/Configuration.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <sys/select.h>
#include <util/buffer/IOBuffer.hpp>
#include <list>
#include <map>

class Logger;

class HTTPOrchestrator
{
	public:
		typedef std::list<HTTPServer*> server_container;
		typedef server_container::iterator server_iterator;

	public:
		static Logger &LOG;

	private:
		const Configuration m_configuration;
		server_container m_servers;
		fd_set m_fds;
		int m_highestFd;
		int m_fdCount;
		std::map<int, HTTPServer*> serverFds;
		std::map<int, IOBuffer*> fileReadFds;
		std::map<int, HTTPClient*> clientFds;
		std::map<int, IOBuffer*> fileWriteFds;

		HTTPOrchestrator(const Configuration &configuration, const server_container &servers);

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
		printSelectOutput(fd_set &readFds, fd_set &writeFds);

		void
		addServerFd(int fd, HTTPServer &server);

		void
		addFileReadFd(int fd, IOBuffer &ioBuffer);

		void
		addClientFd(int fd, HTTPClient &client);

		void
		addFileWriteFd(int fd, IOBuffer &ioBuffer);

		void
		removeFileRead(int fd);

		void
		removeClient(int fd);

	public:
		void
		start();

	public:
		static HTTPOrchestrator
		create(const Configuration &configuration);
};

#endif
