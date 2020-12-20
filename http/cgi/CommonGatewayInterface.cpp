/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:36:04 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 19:36:04 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/CGIBlock.hpp>
#include <exception/IOException.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPMethod.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPServer.hpp>
#include <http/HTTPVersion.hpp>
#include <net/address/InetAddress.hpp>
#include <net/address/InetSocketAddress.hpp>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <util/Convert.hpp>
#include <util/Enum.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <util/Optional.hpp>
#include <util/URL.hpp>
#include <cstdlib>
#include <list>
#include <string>

const std::string CommonGatewayInterface::ENV_AUTH_TYPE = "AUTH_TYPE";
const std::string CommonGatewayInterface::ENV_CONTENT_LENGTH = "CONTENT_LENGTH";
const std::string CommonGatewayInterface::ENV_CONTENT_TYPE = "CONTENT_TYPE";
const std::string CommonGatewayInterface::ENV_GATEWAY_INTERFACE = "GATEWAY_INTERFACE";
const std::string CommonGatewayInterface::ENV_PATH_INFO = "PATH_INFO";
const std::string CommonGatewayInterface::ENV_PATH_TRANSLATED = "PATH_TRANSLATED";
const std::string CommonGatewayInterface::ENV_QUERY_STRING = "QUERY_STRING";
const std::string CommonGatewayInterface::ENV_REMOTE_ADDR = "REMOTE_ADDR";
const std::string CommonGatewayInterface::ENV_REMOTE_PORT = "REMOTE_PORT";
const std::string CommonGatewayInterface::ENV_REMOTE_IDENT = "REMOTE_IDENT";
const std::string CommonGatewayInterface::ENV_REMOTE_USER = "REMOTE_USER";
const std::string CommonGatewayInterface::ENV_REQUEST_METHOD = "REQUEST_METHOD";
const std::string CommonGatewayInterface::ENV_REQUEST_URI = "REQUEST_URI";
const std::string CommonGatewayInterface::ENV_SCRIPT_NAME = "SCRIPT_NAME";
const std::string CommonGatewayInterface::ENV_SERVER_NAME = "SERVER_NAME";
const std::string CommonGatewayInterface::ENV_SERVER_PORT = "SERVER_PORT";
const std::string CommonGatewayInterface::ENV_SERVER_PROTOCOL = "SERVER_PROTOCOL";
const std::string CommonGatewayInterface::ENV_SERVER_SOFTWARE = "SERVER_SOFTWARE";

CommonGatewayInterface::CommonGatewayInterface(HTTPClient &client, const CGIBlock &cgiBlock, const Environment &environment) :
		m_client(client),
		m_cgiBlock(cgiBlock),
		m_environment(environment)
{
}

CommonGatewayInterface::~CommonGatewayInterface()
{
}

void
CommonGatewayInterface::execute()
{
	Environment env = m_environment;

	env.setProperty(ENV_GATEWAY_INTERFACE, "CGI/1.1");
	env.setProperty(ENV_REMOTE_ADDR, m_client.socketAddress().address()->hostAddress());
	env.setProperty(ENV_REMOTE_PORT, Convert::toString(m_client.socketAddress().port()));
	env.setProperty(ENV_REQUEST_METHOD, m_client.request()->method().name());
	env.setProperty(ENV_REQUEST_URI, m_client.request()->url().path());
	env.setProperty(ENV_SCRIPT_NAME, m_cgiBlock.path().get());
	env.setProperty(ENV_SERVER_NAME, m_client.server().host());
	env.setProperty(ENV_SERVER_PORT, Convert::toString(m_client.server().port()));
	env.setProperty(ENV_SERVER_PROTOCOL, m_client.request()->version().format());
	env.setProperty(ENV_SERVER_SOFTWARE, "webserv/1.0");

	char **envp = env.allocate();

	pid_t pid = ::fork();

	if (pid == -1)
	{
		DeleteHelper::pointers<char>(envp);
		throw IOException("fork", errno);
	}

	if (pid == 0)
	{
		std::string path = m_cgiBlock.path().get();

		char *const argv[] = {
			const_cast<char*>(path.c_str()), /* Dangerous, but kernel allocate it anyway...) */
			NULL };

		::execve(m_cgiBlock.path().get().c_str(), argv, envp);
		::exit(0);
		return;
	}
	else
	{
		int status;
		::waitpid(pid, &status, 0);
		// TODO Link FDs
	}

	DeleteHelper::pointers<char>(envp);
}
