/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPOrchestrator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 14:34:10 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 14:34:10 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/ServerBlock.hpp>
#include <exception/IOException.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/HTTPRequestParser.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <io/SocketServer.hpp>
#include <sys/errno.h>
#include <sys/time.h>
#include <util/buffer/IOBuffer.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <utility>

class HTTPResponse;

class FileDescriptorWrapper;

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

typedef struct
{
		FileDescriptorWrapper *fd;
		HTTPRequestParser parser;
		unsigned long last_action;
		HTTPResponse *response;
} client;

void
HTTPOrchestrator::start()
{
	prepare();

	fd_set readFdSet;
	fd_set writeFdSet;

	std::map<int, HTTPServer*> serverFds;
	std::map<int, IOBuffer*> fileReadFds;
	std::map<int, client*> clientFds;
	std::map<int, IOBuffer*> fileWriteFds;

	for (server_iterator it = m_servers.begin(); it != m_servers.end(); it++)
	{
		int fd = it->serverSocket().fd();

		setFd(fd);
		serverFds.insert(serverFds.end(), std::make_pair(fd, &(*it)));
	}

	while (1)
	{
		readFdSet = m_fds;
		writeFdSet = m_fds;

		if (::select(m_highestFd + 1, &readFdSet, &writeFdSet, NULL, NULL) == -1)
			throw IOException("select", errno);

//		{
//			typedef std::map<int, HTTPServer*>::iterator iterator;
//
//			for (iterator it = serverFds.begin(); it != serverFds.end(); it++)
//			{
//				int fd = it->first;
//
//				if (FD_ISSET(fd, &readFdSet))
//				{
//					try
//					{
//						int accepted = ::accept(fd, NULL, NULL);
//						setFd(accepted);
//
//						client *c = new client();
//						c->response = NULL;
//						c->last_action = seconds();
//						c->fd = FileDescriptorWrapper::wrap(accepted);
//						c->fd->setNonBlock();
//
//						clientFds[accepted] = c;
//					}
//					catch (Exception &e)
//					{
//						std::cout << e.what() << std::endl;
//					}
//				}
//			}
//		}

//		{
//			typedef std::map<int, FileDescriptorWrapper*>::iterator iterator;
//
//			std::set<int> fdToRemove;
//
//			for (iterator it = fileReadFds.begin(); it != fileReadFds.end(); it++)
//			{
//				int fd = it->first;
//
//				if (FD_ISSET(fd, &readFdSet))
//				{
//					FileDescriptorWrapper *fdWrapper = it->second;
//
//					if (fdWrapper->fillWithRead() == -1 || fdWrapper->isDone())
//					{
//						std::cout << "fd-read :: remove(" << fd << "): " << ::strerror(errno) << std::endl;
//						fdToRemove.insert(fdToRemove.end(), fd);
//					}
//				}
//			}
//
//			for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
//			{
//				iterator found = fileReadFds.find(*it);
//
//				if (found != fileReadFds.end())
//					fileReadFds.erase(found);
//			}
//		}

//		{
//			typedef std::map<int, client*>::iterator iterator;
//
//			std::set<int> fdToRemove;
//
//			for (iterator it = clientFds.begin(); it != clientFds.end(); it++)
//			{
//				unsigned long now = seconds();
//				int fd = it->first;
//
//				bool canRead = FD_ISSET(fd, &readFdSet);
//				bool canWrite = FD_ISSET(fd, &writeFdSet);
//
//				client *cli = it->second;
//
//				if (canRead && !cli->response)
//				{
//					if (cli->fd->getReadBufferSize() != 0 || cli->fd->fillWithReceive() > 0)
//					{
//						char c;
//
//						while (cli->fd->consume(&c))
//						{
//							cli->parser.consume(c);
//							if (cli->parser.state() == HttpRequestParser::S_END)
//							{
//								std::string file = cli->parser.path().substr(1);
//
//								int ffd = ::open(("." + cli->parser.path()).c_str(), O_RDONLY);
//
//								HTTPHeaderFields header;
//								header.date();
//								header.set("Server", "webserv");
//
//								struct stat st;
//								if (ffd == -1 || ::stat(("." + cli->parser.path()).c_str(), &st) == -1)
//								{
//									header.contentType("text/html");
//									header.contentLength(8);
//									std::cout << "GET " + cli->parser.path() + " -> 404" << std::endl;
//
//									if (ffd != -1)
//										::close(ffd);
//
//									cli->response = new HTTPResponse(HTTPVersion::HTTP_1_1, *HTTPStatus::OK, header, new HTTPResponse::StringBody("not found"));
//								}
//								else
//								{
//									std::cout << "GET " + cli->parser.path() + " -> 200" << std::endl;
//
//									if (S_ISDIR(st.st_mode))
//									{
//										if (ffd != -1)
//											::close(ffd);
//
//										std::string directory = cli->parser.path() == "/" ? "." : file;
//
//										DIR *dir = ::opendir(directory.c_str());
//
//										std::string listing = "";
//
//										struct dirent *entry;
//										while ((entry = ::readdir(dir)))
//										{
//											std::string lfile(entry->d_name);
//											std::string absolute = directory + "/" + lfile;
//
//											if (::stat(absolute.c_str(), &st) != -1 && S_ISDIR(st.st_mode))
//											{
//												lfile += '/';
//											}
//
//											listing += std::string("<a href=\"./") + lfile + "\">" + lfile + "</a><br>";
//										}
//
//										::closedir(dir);
//
//										header.contentType("text/html");
//										header.contentLength(listing.size());
//
//										cli->response = new HTTPResponse(HTTPVersion::HTTP_1_1, *HTTPStatus::OK, header, new HTTPResponse::StringBody(listing));
//									}
//									else if (S_ISREG(st.st_mode))
//									{
//										header.contentType(mimeRegistry, file.substr(file.rfind(".") + 1));
//										header.contentLength(st.st_size);
//
//										cli->response = new HTTPResponse(HTTPVersion::HTTP_1_1, *HTTPStatus::OK, header, new HTTPResponse::FileBody(ffd));
//									}
//								}
//
//								break;
//							}
//						}
//					}
//				}
//
//				if (canWrite)
//				{
//					if (cli->response)
//					{
//						if (!cli->response->write(*(cli->fd))) // TODO Add windows support
//						{
//							cli->last_action = now;
//						}
//						else
//						{
//							std::cout << "closing(" << fd << "): " << ::strerror(errno) << std::endl;
//							::close(fd);
//							delete cli->fd;
//							delete cli->response;
//							delete cli;
//
//							fdToRemove.insert(fdToRemove.end(), fd);
//
//							clearFd(fd);
//							continue;
//						}
//					}
//				}
//
//				if (cli->last_action + 5 < now)
//				{
//					std::cout << "timeout: " << fd << std::endl;
//					::close(fd);
//					delete cli->fd;
//					delete cli->response;
//					delete cli;
//
//					fdToRemove.insert(fdToRemove.end(), fd);
//
//					clearFd(fd);
//					continue;
//				}
//			}
//
//			for (std::set<int>::iterator it = fdToRemove.begin(); it != fdToRemove.end(); it++)
//			{
//				iterator found = clientFds.find(*it);
//
//				if (found != clientFds.end())
//					clientFds.erase(found);
//			}
//		}

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

	Configuration::siterator it = configuration.servers().begin();
	Configuration::siterator ite = configuration.servers().end();

	while (it != ite)
	{
		portToServersMap[it->port().get()].push_back(*it);
		LOG.debug() << "Mapping port " << it->port().get() << " with server: " << it->name().get() << std::endl;
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
