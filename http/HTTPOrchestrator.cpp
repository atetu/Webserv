/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPOrchestrator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:34:10 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/15 15:59:32 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <exception/IOException.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPMethod.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPRequestParser.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <io/SocketServer.hpp>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <util/buffer/BaseBuffer.hpp>
#include <util/buffer/IOBuffer.hpp>
#include <util/Enum.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/System.hpp>
#include <util/URL.hpp>
#include <set>
#include <utility>


#include <http/handler/HTTPMethodHandler.hpp>
#include <http/handler/methods/GetHandler.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPHeaderParser.hpp>
#include <http/HTTPFindLocation.hpp>

class System;

#ifdef __linux__
# include <unistd.h>
#include <sys/stat.h>
#elif __APPLE__
# include <unistd.h>
#include <sys/stat.h>
#elif __CYGWIN__
# include <sys/unistd.h>
#include <cygwin/stat.h>
#include <sys/_default_fcntl.h>
#include <sys/_timeval.h>
#include <sys/dirent.h>
#else
# error Unknown plateform
#endif

#include <util/Optional.hpp>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

unsigned long
seconds()
{
	struct timeval val;
	if (gettimeofday(&val, NULL))
		return (0);

	return (val.tv_sec);
}

Logger &HTTPOrchestrator::LOG = LoggerFactory::get("HTTP Orchestrator");

HTTPOrchestrator::HTTPOrchestrator(const Configuration &configuration, const server_container &servers) :
		m_configuration(configuration),
		m_servers(servers),
		m_fds(),
		m_highestFd(0),
		m_fdCount(0)
{
	FD_ZERO(&m_fds);
}

HTTPOrchestrator::~HTTPOrchestrator()
{
}

void
HTTPOrchestrator::prepare(void)
{
	server_iterator it = m_servers.begin();
	server_iterator ite = m_servers.end();

	while (it != ite)
	{
		try
		{
			it->prepare();
		}
		catch (...)
		{
			unprepare();

			throw;
		}

		it++;
	}
}

void
HTTPOrchestrator::unprepare(void)
{
	server_iterator it = m_servers.begin();
	server_iterator ite = m_servers.end();

	while (it != ite)
	{
		it->unprepare();

		it++;
	}
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

	std::map<int, HTTPServer*> serverFds;
	std::map<int, IOBuffer*> fileReadFds;
	std::map<int, HTTPClient*> clientFds;
	std::map<int, IOBuffer*> fileWriteFds;

	for (server_iterator it = m_servers.begin(); it != m_servers.end(); it++)
	{
		int fd = it->serverSocket().fd();

		setFd(fd);
		serverFds.insert(serverFds.end(), std::make_pair(fd, &(*it)));
	}

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 5000;

	while (1)
	{
		readFdSet = m_fds;
		writeFdSet = m_fds;

		if (::select(m_highestFd + 1, &readFdSet, &writeFdSet, NULL, &timeout) == -1)
			throw IOException("select", errno);

		printSelectOutput(readFdSet, writeFdSet);

		{
			typedef std::map<int, HTTPServer*>::iterator iterator;

			for (iterator it = serverFds.begin(); it != serverFds.end(); it++)
			{
				int serverFd = it->first;

				if (FD_ISSET(serverFd, &readFdSet))
				{
					int accepted = ::accept(serverFd, NULL, NULL);

					if (accepted == -1)
						throw IOException("accept", errno);

					setFd(accepted);

					clientFds[accepted] = new HTTPClient(accepted);
				}
			}
		}

		{
			typedef std::map<int, IOBuffer*>::iterator iterator;

			std::set<int> fdToRemove;

			for (iterator it = fileReadFds.begin(); it != fileReadFds.end(); it++)
			{
				int fd = it->first;

				if (FD_ISSET(fd, &readFdSet))
				{
					IOBuffer *buffer = it->second;

					if (buffer->read() == -1 || buffer->hasReadEverything())
					{
						std::cout << "fd-read :: remove(" << fd << "): " << ::strerror(errno) << std::endl;
						fdToRemove.insert(fdToRemove.end(), fd);
					}
				}
			}

			for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
			{
				iterator found = fileReadFds.find(*it);

				if (found != fileReadFds.end())
					fileReadFds.erase(found);
			}
		}

		{
			typedef std::map<int, HTTPClient*>::iterator iterator;

			std::set<int> fdToRemove;

			for (iterator it = clientFds.begin(); it != clientFds.end(); it++)
			{
				unsigned long now = System::currentTimeSeconds();
				int fd = it->first;

				bool canRead = FD_ISSET(fd, &readFdSet);
				bool canWrite = FD_ISSET(fd, &writeFdSet);

				HTTPClient *client = it->second;

				if (canRead && !client->response())
				{
					if (client->in().size() != 0 || client->in().recv() > 0)
					{
						char c;

						while (client->in().next(c))
						{
							client->parser().consume(c);

							if (client->parser().state() == HTTPRequestParser::S_END || client->parser().state() == HTTPRequestParser::S_CONTINUE)
							{
								// TODO Route matching
								
						
								if (client->parser().state() == HTTPRequestParser::S_CONTINUE)
								{
									char y = client->parser().lastChar();
									
									while (1)
									{
										HTTPHeaderParser headerParser;
										headerParser.consume(y);
						
										while (client->in().next(c))
										{
											headerParser.consume(c);
											if (headerParser.state() == HTTPHeaderParser::S_END || headerParser.state() == HTTPHeaderParser::S_CONTINUE)
												break;
										}
										client->header(headerParser);
										if (headerParser.state() == HTTPHeaderParser::S_END)
											break;
									}
								}
							
								const HTTPMethod *method = HTTPMethod::find(client->parser().method());
							
								HTTPHeaderFields *header = HTTPHeaderFields::create(client->getHeader());
								
								std::string clientHost ;
								std::map<std::string, std::string> headerMap = header->storage();
								std::map<std::string, std::string>::iterator header_it = headerMap.begin();
								std::map<std::string, std::string>::iterator header_ite = headerMap.end();
								
								while (header_it != header_ite)
								{
									if (header_it->first.compare("Host") == 0)
										clientHost = header_it->second;
									header_it++;
								}
								ServerBlock *serverBlock;
								
								std::list<ServerBlock*> serverBlockList = m_configuration.getRootBlock()->server();
								std::list<ServerBlock*>::iterator server_it = serverBlockList.begin();
								std::list<ServerBlock*>::iterator server_ite = serverBlockList.end();
								int found = 0;
								
								while (server_it != server_ite)
								{
									if ((*server_it)->names().present())
									{
										std::list<std::string> serverNames = (*server_it)->names().get();
										std::list<std::string>::iterator serverNames_it = serverNames.begin();
										std::list<std::string>::iterator serverNames_ite = serverNames.end();

										while (serverNames_it != serverNames_ite)
										{
											if (serverNames_it->compare(clientHost) == 0)
											{
												serverBlock = *server_it;
												found = 1;
												break;
											}
											serverNames_it++;
										}
										
									}
									if (found)
										break;
									server_it++;
								}
								
								HTTPFindLocation findLocation(client->parser().path(), serverBlock->locations().get());
								LocationBlock *locationBlock = (findLocation.parse()).location();
								
								if (!method)
									client->response() = HTTPResponse::status(*HTTPStatus::METHOD_NOT_ALLOWED);
								else
								{
									URL url = URL("http", "locahost", 80, client->parser().path(), Optional<std::map<std::string, std::string> >(), Optional<std::string>());

									client->request() = new HTTPRequest(*method, url, HTTPVersion::HTTP_1_1, HTTPHeaderFields(), m_configuration, RootBlock(), ServerBlock(), LocationBlock());
									client->response() = method->handler().handle(*client->request());

									HTTPResponse::FileBody *fileBody = dynamic_cast<HTTPResponse::FileBody*>(client->response()->body());
									if (fileBody)
									{
										IOBuffer &buffer = fileBody->buffer();

										setFd(buffer.fd());
										fileReadFds.insert(fileReadFds.end(), std::make_pair(buffer.fd(), &buffer));
									}
								}

								break;
							}
						}
					}
				}

				if (canWrite)
				{
					if (client->response())
					{
						if (client->out().send() > 0 || !client->response()->write(client->out())) // TODO Add windows support
							client->updateLastAction();
						else
						{
							std::cout << "closing(" << fd << "): " << ::strerror(errno) << std::endl;

							::close(fd);
							delete client;

							fdToRemove.insert(fdToRemove.end(), fd);

							clearFd(fd);
							continue;
						}
					}
				}

				if (client->lastAction() + 5 < now)
				{
					std::cout << "timeout: " << fd << std::endl;

					::close(fd);
					delete client;

					fdToRemove.insert(fdToRemove.end(), fd);

					clearFd(fd);
					continue;
				}
			}

			for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
			{
				iterator found = clientFds.find(*it);

				if (found != clientFds.end())
					clientFds.erase(found);
			}
		}

//		{
//			typedef std::map<int, FileDescriptorWrapper*>::iterator iterator;
//
//			std::set<int> fdToRemove;
//
//			for (iterator it = fileWriteFds.begin(); it != fileWriteFds.end(); it++)
//			{
//				int fd = it->first;
//
//				if (FD_ISSET(fd, &writeFdSet))
//				{
//					FileDescriptorWrapper *fdWrapper = it->second;
//
//					if (fdWrapper->flushWithWrite() == -1 || fdWrapper->isDone())
//					{
//						std::cout << "fd-write :: remove(" << fd << "): " << ::strerror(errno) << std::endl;
//						fdToRemove.insert(fdToRemove.end(), fd);
//					}
//				}
//			}
//
//			for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
//			{
//				iterator found = fileWriteFds.find(*it);
//
//				if (found != fileWriteFds.end())
//					fileWriteFds.erase(found);
//			}
//		}
	}

	/* Should not happen. */
	for (server_iterator it = m_servers.begin(); it != m_servers.end(); it++) // TODO Handle exit
		::close(it->serverSocket().fd());
}

HTTPOrchestrator
HTTPOrchestrator::create(const Configuration &configuration)
{
	typedef std::map<int, std::vector<ServerBlock> >::iterator plsiterator;

	std::map<int, std::vector<ServerBlock> > portToServersMap;

	// Configuration::siterator it = configuration.servers().begin();
	// Configuration::siterator ite = configuration.servers().end();

	std::list<ServerBlock*>::iterator it;
	std::list<ServerBlock*> list = configuration.getRootBlock()->server();
	it = list.begin();
	std::list<ServerBlock*>::iterator ite;
	ite = list.end();
	
	while (it != ite)
	{
		portToServersMap[(*it)->port().get()].push_back(**it);
		LOG.debug() << "Mapping port " << (*it)->port().get() << " with server: " ;//<< (*it)->names().get() << std::endl;
		it++;
	}

	server_container httpServers;
	plsiterator itr = portToServersMap.begin();
	plsiterator itre = portToServersMap.end();

	while (itr != itre)
	{
		httpServers.push_back(HTTPServer(itr->first, itr->second));
		itr++;
	}

	return (HTTPOrchestrator(configuration, httpServers));
}