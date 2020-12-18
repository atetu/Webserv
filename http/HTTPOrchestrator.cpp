/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPOrchestrator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:34:10 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 19:01:03 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <exception/IOException.hpp>
#include <http/handler/HTTPMethodHandler.hpp>
#include <http/HTTPFindLocation.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPMethod.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPRequestParser.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <io/Socket.hpp>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <util/buffer/impl/SocketBuffer.hpp>
#include <util/Enum.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/Optional.hpp>
#include <util/System.hpp>
#include <util/URL.hpp>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <utility>

Logger &HTTPOrchestrator::LOG = LoggerFactory::get("HTTP Orchestrator");

HTTPOrchestrator::HTTPOrchestrator(const Configuration &configuration, const server_container &servers) :
		m_configuration(configuration),
		m_servers(servers),
		m_fds(),
		m_highestFd(0),
		m_fdCount(0),
		serverFds(),
		fileReadFds(),
		clientFds(),
		fileWriteFds()
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

	LOG.debug() << "setFd(" << fd << " / " << m_fdCount << ")" << std::endl;

	if (fd > m_highestFd)
		m_highestFd = fd;
}

void
HTTPOrchestrator::clearFd(int fd)
{
	FD_CLR(fd, &m_fds);
	--m_fdCount;

	LOG.debug() << "clearFd(" << fd << " / " << m_fdCount << ")" << std::endl;

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

	while (1)
	{
		readFdSet = m_fds;
		writeFdSet = m_fds;

		if (::select(m_highestFd + 1, &readFdSet, &writeFdSet, NULL, &timeout) == -1)
			throw IOException("select", errno);

		printSelectOutput(readFdSet, writeFdSet);

		try
		{
			typedef std::map<int, HTTPServer const*>::iterator iterator;

			for (iterator it = serverFds.begin(); it != serverFds.end(); it++)
			{
				int fd = it->first;
				const HTTPServer &httpServer = *it->second;

				if (FD_ISSET(fd, &readFdSet))
					addClient(*(new HTTPClient(*(httpServer.socket().accept()), httpServer)));
			}
		}
		catch (Exception &exception)
		{
			LOG.warn() << "Could not accept connection: " << exception.message() << std::endl;
		}

		try
		{
			typedef std::map<int, FileBuffer*>::iterator iterator;

			std::set<int> fdToRemove;

			for (iterator it = fileReadFds.begin(); it != fileReadFds.end(); it++)
			{
				int fd = it->first;

				if (FD_ISSET(fd, &readFdSet))
				{
					FileBuffer &buffer = *it->second;

					if (buffer.read() == -1 || buffer.hasReadEverything())
					{
						std::cout << "fd-read :: remove(" << fd << " (" << buffer.descriptor().raw() << ")): " << ::strerror(errno) << std::endl;
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

		try
		{
			typedef std::map<int, HTTPClient*>::iterator iterator;

			std::set<int> fdToRemove;

			for (iterator it = clientFds.begin(); it != clientFds.end(); it++)
			{
				unsigned long now = System::currentTimeSeconds();
				int fd = it->first;

				bool canRead = FD_ISSET(fd, &readFdSet);
				bool canWrite = FD_ISSET(fd, &writeFdSet);

				bool deleted = false;

				HTTPClient &client = *it->second;

				if (canRead && !client.response())
				{
					if (client.in().size() != 0 || client.in().recv() > 0)
					{
						char c;

						while (client.in().next(c))
						{
							client.parser().consume(c);

							if (client.parser().state() == HTTPRequestParser::S_END)
							{
								HTTPHeaderFields header = HTTPHeaderFields(client.parser().header()); // isn't enough actually?

								std::map<std::string, std::string>::iterator header_it = header.storage().find("Host");
								if (header_it == header.storage().end())
									throw Exception("No host in header fields");
								std::string clientHost = header_it->second;
								//std::cout << "client : " << clientHost << std::endl;

								const ServerBlock *serverBlock = m_configuration.rootBlock().findServerBlock(clientHost); // ca marche avec inline juste. Pourquoi ?? + explication du const a la fin de fonction?
								//std::cout << "server : " << serverBlock->host().get() << std::endl;

								const LocationBlock *locationBlock = NULL;
								if (serverBlock && serverBlock->locations().present())
								{
									std::cout << serverBlock->locations().present() << std::endl;

									HTTPFindLocation findLocation(client.parser().path(),

									serverBlock->locations().get());

									if (findLocation.parse().location().present())
										locationBlock = findLocation.parse().location().get();
								}

								if (!serverBlock)
									client.response() = HTTPResponse::status(*HTTPStatus::NOT_FOUND);
								else
								{
									//std::cout << "location: " << locationBlock->path() << std::endl;

									const HTTPMethod *method = HTTPMethod::find(client.parser().method());
									if (!method)
										client.response() = HTTPResponse::status(*HTTPStatus::METHOD_NOT_ALLOWED);
									else
									{
										URL url = URL("http", "locahost", 80, client.parser().path(), Optional<std::map<std::string, std::string> >(), Optional<std::string>());

										const RootBlock &rootBlock = m_configuration.rootBlock();

										const HTTPVersion &version = HTTPVersion::HTTP_1_1;
										const Optional<LocationBlock const*> locationBlockOptional = Optional<LocationBlock const*>::ofNullable(locationBlock);

										client.request() = new HTTPRequest(*method, url, version, header, m_configuration, rootBlock, *serverBlock, locationBlockOptional);
										client.response() = method->handler().handle(*client.request());

										HTTPResponse::FileBody *fileBody = dynamic_cast<HTTPResponse::FileBody*>(client.response()->body());
										if (fileBody)
											addFileRead(fileBody->fileBuffer());
									}
								}

								break;
							}
						}
					}
				}

				if (canWrite && !deleted)
				{
					if (client.response())
					{
						if (client.out().send() > 0 || !client.response()->write(client.out()))
							client.updateLastAction();
						else
						{
							std::cout << "closing(" << fd << "): " << ::strerror(errno) << std::endl;

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

				if (!deleted && client.lastAction() + 5 < now)
				{
					std::cout << "timeout: " << fd << std::endl;

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
	}
}

void
HTTPOrchestrator::addServer(HTTPServer &server)
{
	int fd = server.socket().raw();

	setFd(fd);
	serverFds.insert(serverFds.end(), std::make_pair(fd, &server));
}

void
HTTPOrchestrator::addFileRead(FileBuffer &fileBuffer)
{
	int fd = fileBuffer.descriptor().raw();

	setFd(fd);
	fileReadFds.insert(fileReadFds.end(), std::make_pair(fd, &fileBuffer));
}

void
HTTPOrchestrator::addClient(HTTPClient &client)
{
	int fd = client.socket().raw();

	setFd(fd);
	clientFds[fd] = &client;
}

void
HTTPOrchestrator::removeFileRead(int fd)
{
	typedef std::map<int, FileBuffer*>::iterator iterator;

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
	typedef std::map<int, HTTPClient*>::iterator iterator;

	iterator found = clientFds.find(fd);

	if (found != clientFds.end())
	{
		HTTPClient *client = found->second;

		HTTPResponse *response = client->response();
		if (response)
		{
			HTTPResponse::IBody *body = response->body();

			if (body)
			{
				HTTPResponse::FileBody *fileBody = dynamic_cast<HTTPResponse::FileBody*>(body);
				if (fileBody)
				{
					FileBuffer &fileBuffer = fileBody->fileBuffer();
					removeFileRead(fileBuffer.descriptor().raw());
					delete &fileBuffer;
				}
			}
		}

		::close(fd);
		delete client;

		clearFd(fd);

		clientFds.erase(found);
	}
}

HTTPOrchestrator*
HTTPOrchestrator::create(const Configuration &configuration)
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

	return (new HTTPOrchestrator(configuration, httpServers));
}
