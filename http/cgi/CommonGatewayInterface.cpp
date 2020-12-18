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
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <util/Optional.hpp>
#include <cstdlib>
#include <string>

const std::string CommonGatewayInterface::ENV_AUTH_TYPE = "AUTH_TYPE";
const std::string CommonGatewayInterface::ENV_CONTENT_LENGTH = "CONTENT_LENGTH";
const std::string CommonGatewayInterface::ENV_CONTENT_TYPE = "CONTENT_TYPE";
const std::string CommonGatewayInterface::ENV_GATEWAY_INTERFACE = "GATEWAY_INTERFACE";
const std::string CommonGatewayInterface::ENV_PATH_INFO = "PATH_INFO";
const std::string CommonGatewayInterface::ENV_PATH_TRANSLATED = "PATH_TRANSLATED";
const std::string CommonGatewayInterface::ENV_QUERY_STRING = "QUERY_STRING";
const std::string CommonGatewayInterface::ENV_REMOTE_ADDR = "REMOTE_ADDR";
const std::string CommonGatewayInterface::ENV_REMOTE_IDENT = "REMOTE_IDENT";
const std::string CommonGatewayInterface::ENV_REMOTE_USER = "REMOTE_USER";
const std::string CommonGatewayInterface::ENV_REQUEST_METHOD = "REQUEST_METHOD";
const std::string CommonGatewayInterface::ENV_REQUEST_URI = "REQUEST_URI";
const std::string CommonGatewayInterface::ENV_SCRIPT_NAME = "SCRIPT_NAME";
const std::string CommonGatewayInterface::ENV_SERVER_NAME = "SERVER_NAME";
const std::string CommonGatewayInterface::ENV_SERVER_PORT = "SERVER_PORT";
const std::string CommonGatewayInterface::ENV_SERVER_PROTOCOL = "SERVER_PROTOCOL";
const std::string CommonGatewayInterface::ENV_SERVER_SOFTWARE = "SERVER_SOFTWARE";

CommonGatewayInterface::CommonGatewayInterface(const HTTPClient &client, const CGIBlock &cgiBlock, const Environment &environment) :
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
	pid_t pid = ::fork();

	if (pid == -1)
		throw IOException("fork", errno);

	if (pid == 0)
	{


		::execve(m_cgiBlock.path().get().c_str(), NULL, NULL);
		::exit(0);
	}
	else
	{
		// TODO Link FDs
	}
}
