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

#include <config/block/auth/BasicAuthBlock.hpp>
#include <signal.h>
#include <exception/IOException.hpp>
#include <http/cgi/CommonGatewayInterface.hpp>
#include <http/enums/HTTPMethod.hpp>
#include <http/enums/HTTPVersion.hpp>
#include <http/header/HTTPHeaderFields.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPServer.hpp>
#include <http/request/HTTPRequest.hpp>
#include <io/File.hpp>
#include <net/address/InetAddress.hpp>
#include <net/address/InetSocketAddress.hpp>
#include <os/detect_platform.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <util/Convert.hpp>
#include <util/Enum.hpp>
#include <util/helper/DeleteHelper.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/Optional.hpp>
#include <util/StringUtils.hpp>
#include <util/URL.hpp>
#include <webserv.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>

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
const std::string CommonGatewayInterface::ENV_SCRIPT_FILENAME = "SCRIPT_FILENAME";
const std::string CommonGatewayInterface::ENV_SCRIPT_NAME = "SCRIPT_NAME";
const std::string CommonGatewayInterface::ENV_SERVER_NAME = "SERVER_NAME";
const std::string CommonGatewayInterface::ENV_SERVER_PORT = "SERVER_PORT";
const std::string CommonGatewayInterface::ENV_SERVER_PROTOCOL = "SERVER_PROTOCOL";
const std::string CommonGatewayInterface::ENV_SERVER_SOFTWARE = "SERVER_SOFTWARE";

#if IS_CYGWIN
extern int
kill(pid_t pid, int sig);

int
kill(pid_t pid, int sig)
{
	std::cout << "kill(pid, sig) not available with cygwin" << std::endl;
	std::cout << *((char*)NULL); /* Crash */
	return (0);
}
#endif

Logger &CommonGatewayInterface::LOG = LoggerFactory::get("CGI");

CommonGatewayInterface::CommonGatewayInterface(pid_t pid, FileDescriptor &in, FileDescriptor &out) :
		m_pid(pid),
		m_in(in),
		m_out(out)
{
}

CommonGatewayInterface::~CommonGatewayInterface()
{
	delete &m_in;
	delete &m_out;
}

void
CommonGatewayInterface::exit()
{
	::kill(m_pid, SIGKILL);
}

bool
CommonGatewayInterface::running()
{
	if (::kill(m_pid, 0) == 0)
		return (true);

	errno = 0;

	return (false);
}

#define HERE __FUNCTION__ << ':' << __LINE__

CommonGatewayInterface*
CommonGatewayInterface::execute(HTTPClient &client, const CGIBlock &cgiBlock, const Environment &environment)
{
	int inPipe[2];
	if (pipe(inPipe) == -1)
		throw IOException("pipe (in)", errno);

	int outPipe[2];
	if (pipe(outPipe) == -1)
	{
		int err = errno;

		::close(inPipe[0]);
		::close(inPipe[1]);

		throw IOException("pipe (out)", err);
	}

	Environment env = environment;

	if (cgiBlock.environment().present())
	{
		typedef std::map<std::string, std::string> env_map;

		const env_map &customEnvVars = cgiBlock.environment().get();

		for (env_map::const_iterator it = customEnvVars.begin(); it != customEnvVars.end(); it++)
			env.setProperty(it->first, it->second);
	}

	HTTPRequest &request = *client.request();

	File scriptFile(request.url().path());
	File scriptAbsoluteFile(request.root(), scriptFile);

	env.setProperty(ENV_GATEWAY_INTERFACE, "CGI/1.1");
	env.setProperty(ENV_REMOTE_ADDR, client.socketAddress().address()->hostAddress());
	env.setProperty(ENV_REMOTE_PORT, Convert::toString(client.socketAddress().port()));
	env.setProperty(ENV_REQUEST_METHOD, request.method().name());
	env.setProperty(ENV_REQUEST_URI, request.url().path());
	env.setProperty(ENV_SCRIPT_FILENAME, scriptAbsoluteFile.path());
	env.setProperty(ENV_SCRIPT_NAME, scriptFile.path());
	env.setProperty(ENV_SERVER_NAME, client.server().host());
	env.setProperty(ENV_SERVER_PORT, Convert::toString(client.server().port()));
	env.setProperty(ENV_SERVER_PROTOCOL, request.version().format());
	env.setProperty(ENV_SERVER_SOFTWARE, APPLICATION_NAME_AND_VERSION);
	env.setProperty(ENV_PATH_INFO, scriptFile.parent().path());
	env.setProperty(ENV_PATH_TRANSLATED, scriptAbsoluteFile.parent().path());
	env.setProperty(ENV_QUERY_STRING, request.url().queryString());

	if (request.method().hasBody())
	{
		env.setProperty(ENV_CONTENT_LENGTH, Convert::toString(request.body().length()));

		Optional<std::string> optional = request.headers().get(HTTPHeaderFields::CONTENT_TYPE);
		if (optional.present())
			env.setProperty(ENV_CONTENT_TYPE, optional.get());
	}

	if (request.needAuth()) // TODO Need to be changed to authorization objects
	{
		const AuthBlock *authBlock = request.auth();

		if (authBlock)
		{
			env.setProperty(ENV_AUTH_TYPE, authBlock->prettyType());

			const BasicAuthBlock *basicAuthBlock = dynamic_cast<BasicAuthBlock const*>(authBlock);
			if (basicAuthBlock)
			{
				env.setProperty(ENV_REMOTE_USER, basicAuthBlock->user());
				env.setProperty(ENV_REMOTE_IDENT, basicAuthBlock->user());
			}
		}
	}

	const HTTPHeaderFields &headers = request.headers();
	for (HTTPHeaderFields::const_iterator it = headers.begin(); it != headers.end(); it++)
		env.setProperty("HTTP_" + StringUtils::toUpperCase(StringUtils::replace(StringUtils::replace(it->first, '=', '_'), '-', '_')), it->second);

	char **envp = env.allocate();

	pid_t pid = ::fork();
	if (pid == -1)
	{
		int err = errno;

		DeleteHelper::pointers2<char>(envp);

		::close(inPipe[0]);
		::close(inPipe[1]);
		::close(outPipe[0]);
		::close(outPipe[1]);

		throw IOException("fork", err);
	}

	if (pid == 0)
	{
		::chdir(request.root().c_str());

		::dup2(inPipe[0], 0);
		::dup2(outPipe[1], 1);

//		if (cgiBlock.redirectErrToOut().orElse(true)) // TODO Need debug
//			::dup2(1, 2);

		std::string path = cgiBlock.path().get();
		std::string file = "." + request.url().path();

		char *const argv[] = {
			const_cast<char*>(path.c_str()), /* Dangerous, but kernel allocate it anyway... */
			const_cast<char*>(file.c_str()),
			NULL };

		::execve(path.c_str(), argv, envp);
		::exit(1);
		return (NULL); /* Should not happen. */
	}
	else // TODO This function is taking care of too much things. Need a split or a rewrite.
	{
		DeleteHelper::pointers2<char>(envp);

		::close(inPipe[0]);
		::close(outPipe[1]);

		FileDescriptor *stdin = NULL;
		FileDescriptor *stdout = NULL;

		try
		{
			stdin = FileDescriptor::wrap(inPipe[1]);
			stdout = FileDescriptor::wrap(outPipe[0]);
		}
		catch (...)
		{
			::kill(pid, SIGKILL);

			if (stdin)
				delete stdin;
			else
				::close(inPipe[1]);

			if (stdout) /* Just in case, but should not happen. */
				delete stdout;
			else
				::close(outPipe[0]);

			throw;
		}

		CommonGatewayInterface *cgi = new CommonGatewayInterface(pid, *stdin, *stdout);

		if (request.method().hasBody()) // TODO Operation is blocking
		{
			cgi->in().write(request.body().c_str(), request.body().length());
			//::write(1, request.body().c_str(), request.body().length());
		}

		return (cgi);
	}
}
