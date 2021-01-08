/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPOrchestrator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:34:10 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/05 16:53:35 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <exception/IOException.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <io/Socket.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <nio/NIOSelector.hpp>
#include <sys/errno.h>
#include <util/Environment.hpp>
#include <util/FileDescriptorSet.hpp>
#include <util/Optional.hpp>
#include <util/Singleton.hpp>
#include <util/System.hpp>
#include <iostream>
#include <new>
#include <string>

class NIOSelector;

Logger &HTTPOrchestrator::LOG = LoggerFactory::get("Orchestrator");

HTTPOrchestrator::HTTPOrchestrator(const Environment &environment, const server_container &servers) :
		m_environment(environment),
		m_servers(servers),
		m_fds(),
		m_highestFd(0),
		m_fdCount(0),
		serverFds(),
		fileReadFds(),
		clientFds(),
		fileWriteFds(),
		m_running(false),
		m_stopping(false)
{
	FD_ZERO(&m_fds);
}

HTTPOrchestrator::~HTTPOrchestrator()
{
}

void
HTTPOrchestrator::prepare(void)
{
	for (server_iterator it = m_servers.begin(); it != m_servers.end(); it++)
	{
		try
		{
			HTTPServer &httpServer = *(*it);

			httpServer.start();
		}
		catch (...)
		{
			unprepare();

			throw;
		}
	}
}

void
HTTPOrchestrator::unprepare(void)
{
	for (server_iterator it = m_servers.begin(); it != m_servers.end(); it++)
		(*it)->terminate();
}

void
HTTPOrchestrator::printSelectOutput(fd_set &readFds, fd_set &writeFds, bool forced)
{
	if (!LOG.isDebugEnabled())
		return;

	static std::string last;
	static unsigned long lastTime = System::currentTimeSeconds();

	std::string line;
	line.reserve(m_highestFd + 1);

	for (int i = 0; i < m_highestFd; i++)
	{
		char c = FD_ISSET(i, &m_fds) ? '-' : '.';

		if (FD_ISSET(i, &writeFds) && FD_ISSET(i, &readFds))
			c = 'X';
		else if (FD_ISSET(i, &readFds))
			c = 'R';
		else if (FD_ISSET(i, &writeFds))
			c = 'W';

		line += c;
	}

	unsigned long now = System::currentTimeSeconds();

	if (forced || line != last || lastTime + 3 < now)
	{
		last = line;
		lastTime = now;

		LOG.debug() << line << std::endl;
	}
}

void
HTTPOrchestrator::start()
{
	prepare();

	for (server_iterator it = m_servers.begin(); it != m_servers.end(); it++)
	{
		HTTPServer &httpServer = *(*it);

		NIOSelector::instance().add(httpServer.socket(), httpServer, NIOSelector::ACCEPT);
	}

	try
	{
		FileDescriptorSet readSet;
		FileDescriptorSet writeSet;

		struct timeval timeout = {
			.tv_sec = 0,
			.tv_usec = 5000 };

		m_running = true;
		while (m_running)
		{
			int fdCount;
			if ((fdCount = NIOSelector::instance().select(&readSet, &writeSet, &timeout)) == -1)
			{
				if (m_stopping && errno == EINTR)
					continue;

				NIOSelector::instance().debug(LOG, readSet, writeSet, true);

				throw IOException("select", errno);
			}

			if (fdCount)
				NIOSelector::instance().debug(LOG, readSet, writeSet, false);
			else if (m_stopping)
			{
				if (!NIOSelector::instance().fds().empty())
				{
					m_running = false;
					break;
				}
			}
		}
	}
	catch (Exception &exception)
	{
		LOG.error() << exception.message() << std::endl;
	}
	catch (std::bad_alloc &exception)
	{
		LOG.error() << exception.what() << std::endl;
	}

	m_running = false;

	while (!m_servers.empty())
	{
		server_iterator it = m_servers.begin();
		HTTPServer &httpServer = *(*it);

		try
		{
			NIOSelector::instance().remove(httpServer.socket());
			httpServer.terminate();
		}
		catch (Exception &exception)
		{
			LOG.error() << "Failed to terminate: " << httpServer.host() << ":" << httpServer.port() << ": " << exception.message() << std::endl;
		}

		delete &httpServer;

		m_servers.erase(it);
	}
}

void
HTTPOrchestrator::terminate()
{
//	m_stopping = true;
//
//	typedef std::map<int, HTTPServer const*>::iterator iterator;
//
//	std::set<int> fdToRemove;
//	for (iterator it = serverFds.begin(); it != serverFds.end(); it++)
//		fdToRemove.insert(fdToRemove.end(), it->first);
//
//	for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
//		removeServer(*it);
}

HTTPOrchestrator*
HTTPOrchestrator::create(const Environment &environment)
{
	typedef std::map<short, std::list<ServerBlock const*> > port_map;
	typedef port_map::const_iterator port_iterator;

	typedef std::map<std::string, port_map> host_map;
	typedef host_map::const_iterator host_iterator;

	host_map hostToPortToServersMap;

	const RootBlock &rootBlock = Configuration::instance().rootBlock();

	const RootBlock::slist serverBlocks = rootBlock.serverBlocks().get();
	for (RootBlock::sciterator sit = serverBlocks.begin(); sit != serverBlocks.end(); sit++)
	{
		const ServerBlock &serverBlock = *(*sit);

		const std::string host = serverBlock.host().orElse(ServerBlock::DEFAULT_HOST);
		const short port = serverBlock.port().orElse(ServerBlock::DEFAULT_PORT);

		hostToPortToServersMap[host][port].push_back(&serverBlock);
	}

	server_container httpServers;
	for (host_iterator hit = hostToPortToServersMap.begin(); hit != hostToPortToServersMap.end(); hit++)
	{
		const std::string &host = hit->first;
		const port_map &portMap = hit->second;

		for (port_iterator pit = portMap.begin(); pit != portMap.end(); pit++)
		{
			short port = pit->first;
			const std::list<ServerBlock const*> &serverBlocks = pit->second;

			httpServers.push_back(new HTTPServer(host, port, serverBlocks));
		}
	}

	return (new HTTPOrchestrator(environment, httpServers));
}
