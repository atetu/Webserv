/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPOrchestrator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:34:10 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 21:28:16 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <exception/IOException.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPStatus.hpp>
#include <http/handler/HTTPMethodHandler.hpp>
#include <http/header/HTTPDate.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/request/HTTPRequest.hpp>
#include <http/request/HTTPRequestProcessor.hpp>
#include <http/parser/HTTPRequestParser.hpp>
#include <http/response/HTTPResponse.hpp>
#include <http/response/HTTPStatusLine.hpp>
#include <http/response/impl/generic/GenericHTTPResponse.hpp>
#include <io/Socket.hpp>
#include <net/address/InetAddress.hpp>
#include <net/address/InetSocketAddress.hpp>
#include <sys/errno.h>
#include <sys/types.h>
#include <buffer/impl/SocketBuffer.hpp>
#include <util/Enum.hpp>
#include <util/Environment.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <util/Optional.hpp>
#include <util/System.hpp>
#include <util/URL.hpp>
#include <cstring>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

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
								httpClient.response() = HTTPMethodHandler::status(*HTTPStatus::SERVICE_UNAVAILABLE, HTTPHeaderFields().retryAfter(10));
						//	std::cout << "NewClient \n";
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

						if (canRead && !client.response())
						{
						//	std::cout << "ici\n";
							
							// std::cout << "size: " << client.in().size() << std::endl;
							// std::cout << "size: " << client.in().recv() << std::endl;
							
							if (client.in().size() != 0 || client.in().recv() > 0)
							{
								char c;
								bool toContinue = false;
								while (client.in().next(c))
								{
									if (!toContinue)
									{
										
									
									try
									{
								//	std::cout << "consume\n";
										client.parser().consume(c);
										if (client.parser().state() == HTTPRequestParser::S_END && c != '\n')
										{
											client.in().first(c); // I know you will have a panick attack when reading that but that's a temporary solution. I dont know it is not directly taken into account below
										}
									}
									catch (Exception &exception)
									{
										LOG.debug() << exception.message() << std::endl;

										client.response() = GenericHTTPResponse::status(*HTTPStatus::BAD_REQUEST);
									}
									}
									if (!client.response())
									{
										try
										{
											// std::cout << "end\n";
											// std::cout << "c: " << c << std::endl;
											if (client.parser().state() == HTTPRequestParser::S_END)
											{	
											//	std::cout <<"processor\n";
												if (!HTTPRequestProcessor(m_configuration, m_environment).process(client))
												{
													toContinue = true;
												//	std::cout << "continue\n";
													continue;
												}
												else
												{
													toContinue = false;
												}
												
											}
										}
										catch (Exception &exception)
										{
											client.response() = GenericHTTPResponse::status(*HTTPStatus::INTERNAL_SERVER_ERROR);
										}
									}

									if (client.response())
									{
										HTTPResponse::fdb_vector buffers;

										client.response()->readFileDescriptors(buffers);
										for (HTTPResponse::fdb_iterator it = buffers.begin(); it != buffers.end(); it++)
										{
//											std::cout << "read\n";
											addFileDescriptorBufferRead(*(*it));
										}

										buffers.clear();

										client.response()->writeFileDescriptors(buffers);
										for (HTTPResponse::fdb_iterator it = buffers.begin(); it != buffers.end(); it++)
											addFileDescriptorBufferWrite(*(*it));

										break;
									}
								}
							}
						}

						if (canWrite && !deleted)
						{
							if (client.response())
							{
								ssize_t sent = client.out().send();
								if (sent != -1 && (sent > 0 || !client.response()->write(client.out())))
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

						if (!deleted && client.response() && client.response()->state() == HTTPResponse::FINISHED)
						{
							std::cout << "done: " << fd << std::endl;

							fdToRemove.insert(fdToRemove.end(), fd);

							deleted = true;

							continue;
						}
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

					if (client.lastAction() + 30 /* TODO */ < now)
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

		if (client.response())
		{
			HTTPResponse::fdb_vector buffers;

			client.response()->readFileDescriptors(buffers);
			for (HTTPResponse::fdb_iterator it = buffers.begin(); it != buffers.end(); it++)
				removeFileRead((*it)->descriptor().raw());

			buffers.clear();

			client.response()->writeFileDescriptors(buffers);
			for (HTTPResponse::fdb_iterator it = buffers.begin(); it != buffers.end(); it++)
				removeFileWrite((*it)->descriptor().raw());

			if (client.request() && LOG.isInfoEnabled())
			{
				LOG.info() << '[' << HTTPDate::now().format() << "] "
				/**/<< client.socketAddress().hostAddress()
				/**/<< " - "
				/**/<< client.request()->method().name()
				/**/<< " "
				/**/<< client.request()->url().path()
				/**/<< " :: "
				/**/<< client.response()->statusLine().status().code()
				/**/<< std::endl;
			}
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
