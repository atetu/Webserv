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
#include <dirent.h>
#include <exception/IOException.hpp>
#include <http/HttpRequestParser.hpp>
#include <http/HttpResponse.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <io/FileDescriptorWrapper.hpp>
#include <io/SocketServer.hpp>
#include <sys/errno.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>

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
#include <sys/types.h>
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

HTTPOrchestrator::HTTPOrchestrator(const Configuration &configuration, const std::vector<HTTPServer> &servers) :
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
	iterator it = m_servers.begin();
	iterator ite = m_servers.end();

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
	iterator it = m_servers.begin();
	iterator ite = m_servers.end();

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

	std::cout << "setFd(" << fd << " / " << m_fdCount << ")" << std::endl;

	if (fd > m_highestFd)
		m_highestFd = fd;
}

void
HTTPOrchestrator::clearFd(int fd)
{
	FD_CLR(fd, &m_fds);
	--m_fdCount;

	std::cout << "clearFd(" << fd << " / " << m_fdCount << ")" << std::endl;

	if (fd == m_highestFd)
		m_highestFd--;
}

typedef struct
{
		FileDescriptorWrapper *fd;
		HttpRequestParser parser;
		unsigned long last_action;
		HttpResponse *response;
} client;

void
HTTPOrchestrator::start()
{
	prepare();

	MimeRegistry mimeRegistry;
	mimeRegistry.loadFromFile("mime.json");

	for (iterator it = m_servers.begin(); it != m_servers.end(); it++)
		setFd(it->serverSocket().fd());

	fd_set read_fds;
	fd_set write_fds;

	int startAt = m_highestFd + 1;

	std::map<int, client*> clients;

	while (1)
	{
		read_fds = m_fds;
		write_fds = m_fds;

		if (::select(m_highestFd + 1, &read_fds, &write_fds, NULL, NULL) == -1)
			throw IOException("select", errno);

		for (iterator it = m_servers.begin(); it != m_servers.end(); it++)
		{
			int serverFd = it->serverSocket().fd();

			if (FD_ISSET(serverFd, &read_fds))
			{
				try
				{
					int socketFd = ::accept(serverFd, NULL, NULL);

					std::cout << "connected(" << socketFd << ")" << std::endl;

					setFd(socketFd);

					client *c = new client();
					c->response = NULL;
					c->last_action = seconds();
					c->fd = FileDescriptorWrapper::wrap(socketFd);
					c->fd->setNonBlock();

					clients[socketFd] = c;
				}
				catch (Exception &e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}

		unsigned long now = seconds();

		for (int fd = startAt; fd <= m_highestFd; fd++)
		{
			if (!FD_ISSET(fd, &m_fds))
				continue;

			std::map<int, client*>::iterator it = clients.find(fd);

			if (it == clients.end())
			{
				std::cout << "orphane: " << fd << std::endl;
				::close(fd);
				clients.erase(it);

				clearFd(fd);
				continue;
			}

			bool canRead = FD_ISSET(fd, &read_fds);
			bool canWrite = FD_ISSET(fd, &write_fds);

			client *cli = it->second;

			if (cli->last_action + 5 < now)
			{
				std::cout << "timeout: " << fd << std::endl;
				::close(fd);
				delete cli->fd;
				delete cli->response;
				delete cli;
				clients.erase(it);

				clearFd(fd);
				continue;
			}

			if (canRead && !cli->response)
			{
				if (cli->fd->getReadBufferSize() != 0 || cli->fd->fillWithReceive() > 0)
				{
//					if (c == '\n')
//						std::cout << "\\n\n";
//					else if (c == '\r')
//						std::cout << "\\r";
//					else
//						std::cout << c;
//					std::cout << std::flush;

					char c;
//					std::cout << (cli->fd->consume(&c) ? "true" : "false") << (int)c << std::endl;

					while (cli->fd->consume(&c))
					{
						cli->parser.consume(c);
						if (cli->parser.state() == HttpRequestParser::S_END)
						{
							std::string file = cli->parser.path().substr(1);

							int ffd = ::open(("." + cli->parser.path()).c_str(), O_RDONLY);

							HTTPHeaderFields header;
							header.date();
							header.set("Server", "webserv");

							struct stat st;
							if (ffd == -1 || ::stat(("." + cli->parser.path()).c_str(), &st) == -1)
							{
								header.contentType("text/html");
								header.contentLength(8);
								std::cout << "GET " + cli->parser.path() + " -> 404" << std::endl;

								if (ffd != -1)
									::close(ffd);

								cli->response = new HttpResponse(HTTPVersion::HTTP_1_1, *HTTPStatus::OK, header, new HttpResponse::StringBody("not found"));
							}
							else
							{
								std::cout << "GET " + cli->parser.path() + " -> 200" << std::endl;

								if (S_ISDIR(st.st_mode))
								{
									if (ffd != -1)
										::close(ffd);

//									std::cout << file << std::endl;

									std::string directory = cli->parser.path() == "/" ? "." : file;

									DIR *dir = ::opendir(directory.c_str());
//									std::cout << dir << std::endl;

									std::string listing = "";

									struct dirent *entry;
									while ((entry = ::readdir(dir)))
									{
										std::string lfile(entry->d_name);
										std::string absolute = directory + "/" + lfile;

										if (::stat(absolute.c_str(), &st) != -1 && S_ISDIR(st.st_mode))
										{
											lfile += '/';
										}

										listing += std::string("<a href=\"./") + lfile + "\">" + lfile + "</a><br>";
									}

									::closedir(dir);

									header.contentType("text/html");
									header.contentLength(listing.size());

									cli->response = new HttpResponse(HTTPVersion::HTTP_1_1, *HTTPStatus::OK, header, new HttpResponse::StringBody(listing));
								}
								else if (S_ISREG(st.st_mode))
								{
									header.contentType(mimeRegistry, file.substr(file.rfind(".") + 1));
									header.contentLength(st.st_size);

									cli->response = new HttpResponse(HTTPVersion::HTTP_1_1, *HTTPStatus::OK, header, new HttpResponse::FileBody(ffd));
								}
							}

							break;
						}
					}
				}
//				std::cout << "can read(" << fd << ")" << std::endl;
			}

			if (canWrite)
			{
				if (cli->response)
				{
					if (!cli->response->write(*(cli->fd))) // TODO Add windows support
					{
						cli->last_action = seconds();
					}
					else
					{
						std::cout << "closing(" << fd << "): " << ::strerror(errno) << std::endl;
						::close(fd);
						delete cli->fd;
						delete cli->response;
						delete cli;
						clients.erase(it);

						clearFd(fd);
						continue;
					}
				}
//				send(fd, "Hello\n", 6, 0);
//				std::cout << "can write(" << fd << ")" << std::endl;
			}
		}
	}

	/* Should not happen. */
	for (iterator it = m_servers.begin(); it != m_servers.end(); it++) // TODO Handle exit
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
		it++;
	}

	std::vector<HTTPServer> httpServers;
	plsiterator itr = portToServersMap.begin();
	plsiterator itre = portToServersMap.end();

	while (itr != itre)
	{
		httpServers.push_back(HTTPServer(itr->first, itr->second));
		itr++;
	}

	return (HTTPOrchestrator(configuration, httpServers));
}
