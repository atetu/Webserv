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

#include <buffer/impl/SocketBuffer.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <exception/IOException.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/filter/FilterChain.hpp>
#include <http/handler/HTTPMethodHandler.hpp>
#include <http/header/HTTPDate.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/response/body/IResponseBody.hpp>
#include <http/response/HTTPResponse.hpp>
#include <io/Socket.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <net/address/InetAddress.hpp>
#include <net/address/InetSocketAddress.hpp>
#include <sys/errno.h>
#include <sys/types.h>
#include <util/Environment.hpp>
#include <util/Optional.hpp>
#include <util/System.hpp>
#include <util/URL.hpp>
#include <cstring>
#include <iostream>
#include <new>
#include <set>
#include <string>
#include <utility>

Logger &HTTPOrchestrator::LOG = LoggerFactory::get("Orchestrator");

HTTPOrchestrator::HTTPOrchestrator(const Configuration &configuration, const Environment &environment, const server_container &servers) :
		m_configuration(configuration),
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

			addServer(httpServer);
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
HTTPOrchestrator::setFd(int fd)
{
	FD_SET(fd, &m_fds);
	++m_fdCount;

//	LOG.debug() << "setFd(" << fd << " / " << m_fdCount << ")" << std::endl;

	if (fd > m_highestFd)
		m_highestFd = fd;
}

void
HTTPOrchestrator::clearFd(int fd)
{
	FD_CLR(fd, &m_fds);
	--m_fdCount;

//	LOG.debug() << "clearFd(" << fd << " / " << m_fdCount << ")" << std::endl;

	if (fd == m_highestFd)
		m_highestFd--;
}

void
HTTPOrchestrator::printSelectOutput(fd_set &readFds, fd_set &writeFds)
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

	if (line != last || lastTime + 3 < now)
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

	fd_set readFdSet;
	fd_set writeFdSet;

	struct timeval timeout = {
		.tv_sec = 0,
		.tv_usec = 5000 };

	try
	{
		m_running = true;
		while (m_running)
		{
			readFdSet = m_fds;
			writeFdSet = m_fds;

			int fdCount;
			if ((fdCount = ::select(m_highestFd + 1, &readFdSet, &writeFdSet, NULL, &timeout)) == -1)
			{
				if (m_stopping && errno == EINTR)
					continue;

				throw IOException("select", errno);
			}

			printSelectOutput(readFdSet, writeFdSet);

			if (fdCount)
			{
				try
				{
					typedef std::map<int, HTTPServer const*>::iterator iterator;

					for (iterator it = serverFds.begin(); it != serverFds.end(); it++)
					{
						int fd = it->first;
						const HTTPServer &httpServer = *it->second;

						if (FD_ISSET(fd, &readFdSet))
						{
							InetSocketAddress socketAddress;
							Socket *socket = httpServer.socket().accept(&socketAddress);

							HTTPClient &httpClient = *(new HTTPClient(*socket, socketAddress, httpServer));

							if (clientFds.size() >= (unsigned long)m_configuration.rootBlock().maxActiveClient().orElse(RootBlock::DEFAULT_MAX_ACTIVE_CLIENT))
							{
								httpClient.response().status(*HTTPStatus::SERVICE_UNAVAILABLE);
								httpClient.response().headers().retryAfter(10);
							}

							addClient(httpClient);
						}
					}
				}
				catch (Exception &exception)
				{
					LOG.warn() << "Could not accept connection: " << exception.message() << std::endl;
				}

				try
				{
					typedef std::map<int, FileDescriptorBuffer*>::iterator iterator;

					std::set<int> fdToRemove;

					for (iterator it = fileReadFds.begin(); it != fileReadFds.end(); it++)
					{
						int fd = it->first;

						if (FD_ISSET(fd, &readFdSet))
						{
							FileDescriptorBuffer &buffer = *it->second;

							if (buffer.read() == -1 || buffer.hasReadEverything())
							{
								//std::cout << "fd-read :: remove(" << fd << " (" << buffer.descriptor().raw() << ")): " << ::strerror(errno) << std::endl;
								fdToRemove.insert(fdToRemove.end(), fd);
							}
						}
					}

					for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
						removeFileRead(*it);
				}
				catch (Exception &exception)
				{
					LOG.warn() << "Could not handle file reading: " << exception.message() << std::endl;
				}

				if (errno)
				{
					std::cout << __FILE__ << ":" << __LINE__ << " -- " << ::strerror(errno) << std::endl;
					errno = 0;
				}

				try
				{
					typedef std::map<int, HTTPClient*>::iterator iterator;

					std::set<int> fdToRemove;

					for (iterator it = clientFds.begin(); it != clientFds.end(); it++)
					{
						int fd = it->first;

						bool canRead = FD_ISSET(fd, &readFdSet);
						bool canWrite = FD_ISSET(fd, &writeFdSet);

						bool deleted = false;

						HTTPClient &client = *it->second;

						if (canRead && client.response().status().absent())
						{
							if (client.in().size() != 0 || client.in().recv() > 0)
							{
								char c;

								while (client.in().next(c))
								{
//									std::cout << c;
									try
									{
										client.parser().consume(c);
									}
									catch (Exception &exception)
									{
										LOG.debug() << exception.message() << std::endl;

										client.response().status(*HTTPStatus::BAD_REQUEST);
									}

									if (client.response().status().absent())
									{
										try
										{
											if (client.parser().state() == HTTPRequestParser::S_END)
											{
												client.request() = HTTPRequest(m_configuration, client.parser().url());
												client.filterChain().doChaining();

												if (client.response().status().absent() && client.request().method().present() && client.request().method().get()->hasBody())
													client.parser().state() = HTTPRequestParser::S_BODY;

												if (!client.response().body())
													client.filterChain().doChaining();
											}
										}
										catch (Exception &exception)
										{
											client.response().status(*HTTPStatus::INTERNAL_SERVER_ERROR);
										}
									}

									if (client.response().body())
									{
										FileDescriptorBuffer *read = NULL;
										FileDescriptorBuffer *write = NULL;

										client.response().body()->io(&read, &write);

										if (read)
											addFileDescriptorBufferRead(*read);

										if (write)
											addFileDescriptorBufferRead(*write);

										break;
									}
								}
							}
						}

						if (canWrite && !deleted)
						{
							if (client.response().ended())
							{
								ssize_t sent = client.out().send();
								if (sent != -1 && (sent > 0 || !client.response().store(client.out())))
									client.updateLastAction();
								else
								{
									//								std::cout << "closing(" << fd << "): " << ::strerror(errno) << std::endl;

									fdToRemove.insert(fdToRemove.end(), fd);

									deleted = true;

									continue;
								}
							}
						}

//						if (!deleted && client.response().ended() && client.response()->state() == HTTPResponse::S_FLUSH)
//						{
//							std::cout << "done: " << fd << std::endl;
//
//							fdToRemove.insert(fdToRemove.end(), fd);
//
//							deleted = true;
//
//							continue;
//						}
					}

					for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
						removeClient(*it);
				}
				catch (Exception &exception)
				{
					LOG.warn() << "Could not handle client: " << exception.message() << std::endl;
				}

				try
				{
					typedef std::map<int, FileDescriptorBuffer*>::iterator iterator;

					std::set<int> fdToRemove;

					for (iterator it = fileWriteFds.begin(); it != fileWriteFds.end(); it++)
					{
						int fd = it->first;

						if (FD_ISSET(fd, &writeFdSet))
						{
							FileDescriptorBuffer &buffer = *it->second;

							if (buffer.write() == -1)
							{
								// std::cout << "fd-write :: remove(" << fd << " (" << buffer.descriptor().raw() << ")): " << ::strerror(errno) << std::endl;
								fdToRemove.insert(fdToRemove.end(), fd);
							}
						}
					}

					for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
						removeFileWrite(*it);
				}
				catch (Exception &exception)
				{
					LOG.warn() << "Could not handle file reading: " << exception.message() << std::endl;
				}
			}
			else if (m_stopping)
			{
				if (clientFds.empty() && fileReadFds.empty() && fileWriteFds.empty())
				{
					m_running = false;
					break;
				}
			}

			try
			{
				typedef std::map<int, HTTPClient*>::iterator iterator;

				std::set<int> fdToRemove;

				unsigned long now = System::currentTimeSeconds();
				for (iterator it = clientFds.begin(); it != clientFds.end(); it++)
				{
					int fd = it->first;
					HTTPClient &client = *it->second;

					if (client.lastAction() + 30 /* TODO */< now)
					{
						std::cout << "timeout: " << fd << std::endl;

						fdToRemove.insert(fdToRemove.end(), fd);

						continue;
					}
				}

				for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
					removeClient(*it);
			}
			catch (Exception &exception)
			{
				LOG.warn() << "Could not handle client (timeout): " << exception.message() << std::endl;
			}
		}

		m_running = false;
	}
	catch (std::bad_alloc &exception)
	{
		LOG.error() << "Failed to allocate (std::bad_alloc): " << exception.what() << std::endl;
	}

	while (!m_servers.empty())
	{
		server_iterator it = m_servers.begin();
		HTTPServer &httpServer = *(*it);

		try
		{
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
HTTPOrchestrator::addServer(HTTPServer &server)
{
	int fd = server.socket().raw();

	LOG.trace() << "++ server(fd=" << fd << ")" << std::endl;

	setFd(fd);
	serverFds.insert(serverFds.end(), std::make_pair(fd, &server));
}

void
HTTPOrchestrator::addFileDescriptorBufferRead(FileDescriptorBuffer &fileDescriptorBuffer)
{
	int fd = fileDescriptorBuffer.descriptor().raw();

	LOG.trace() << "++ file-read(fd=" << fd << ")" << std::endl;

	setFd(fd);
	fileReadFds.insert(fileReadFds.end(), std::make_pair(fd, &fileDescriptorBuffer));
}

void
HTTPOrchestrator::addFileDescriptorBufferWrite(FileDescriptorBuffer &fileDescriptorBuffer)
{
	int fd = fileDescriptorBuffer.descriptor().raw();

	LOG.trace() << "++ file-write(fd=" << fd << ")" << std::endl;

	setFd(fd);
	fileWriteFds.insert(fileWriteFds.end(), std::make_pair(fd, &fileDescriptorBuffer));
}

void
HTTPOrchestrator::addClient(HTTPClient &client)
{
	int fd = client.socket().raw();

	LOG.trace() << "++ client(fd=" << client.socket().raw() << ") @ " << client.socketAddress().address()->hostAddress() << ":" << client.socketAddress().port() << std::endl;

	setFd(fd);
	clientFds[fd] = &client;
}

void
HTTPOrchestrator::removeServer(int fd)
{
	LOG.trace() << "-- server(fd=" << fd << ")" << std::endl;

	typedef std::map<int, HTTPServer const*>::iterator iterator;

	iterator found = serverFds.find(fd);
	if (found != serverFds.end())
	{
		serverFds.erase(found);
		clearFd(fd);
	}
}

void
HTTPOrchestrator::removeFileRead(int fd)
{
	LOG.trace() << "-- file-read(fd=" << fd << ")" << std::endl;

	typedef std::map<int, FileDescriptorBuffer*>::iterator iterator;

	iterator found = fileReadFds.find(fd);

	if (found != fileReadFds.end())
	{
		fileReadFds.erase(found);
		clearFd(fd);
	}
}

void
HTTPOrchestrator::removeClient(int fd)
{
	LOG.trace() << "-- client(fd=" << fd << ")" << std::endl;

	typedef std::map<int, HTTPClient*>::iterator iterator;

	iterator found = clientFds.find(fd);

	if (found != clientFds.end())
	{
		HTTPClient &client = *found->second;

		if (client.response().body())
		{
			FileDescriptorBuffer *read = NULL;
			FileDescriptorBuffer *write = NULL;

			client.response().body()->io(&read, &write);

			if (read)
				removeFileRead(read->descriptor().raw());

			if (write)
				removeFileWrite(write->descriptor().raw());
		}

		if (client.response().ended() && LOG.isInfoEnabled())
		{
			LOG.info() << '[' << HTTPDate::now().format() << "] "
			/**/<< client.socketAddress().hostAddress()
			/**/<< " - "
			/**/<< client.request().method().get()->name() // TODO Unsafe
			/**/<< " "
			/**/<< client.request().url().path()
			/**/<< " :: "
			/**/<< client.response().status().get()->code()
			/**/<< std::endl;
		}

		clearFd(fd);

		delete &client;
		clientFds.erase(found);
	}
}

void
HTTPOrchestrator::removeFileWrite(int fd)
{
	LOG.trace() << "-- file-write(fd=" << fd << ")" << std::endl;

	typedef std::map<int, FileDescriptorBuffer*>::iterator iterator;

	iterator found = fileWriteFds.find(fd);

	if (found != fileWriteFds.end())
	{
		fileWriteFds.erase(found);
		clearFd(fd);
	}
}

void
HTTPOrchestrator::terminate()
{
	m_stopping = true;

	typedef std::map<int, HTTPServer const*>::iterator iterator;

	std::set<int> fdToRemove;
	for (iterator it = serverFds.begin(); it != serverFds.end(); it++)
		fdToRemove.insert(fdToRemove.end(), it->first);

	for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
		removeServer(*it);
}

HTTPOrchestrator*
HTTPOrchestrator::create(const Configuration &configuration, const Environment &environment)
{
	typedef std::map<short, std::list<ServerBlock const*> > port_map;
	typedef port_map::const_iterator port_iterator;

	typedef std::map<std::string, port_map> host_map;
	typedef host_map::const_iterator host_iterator;

	host_map hostToPortToServersMap;

	const RootBlock &rootBlock = configuration.rootBlock();

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

	return (new HTTPOrchestrator(configuration, environment, httpServers));
}
