/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 15:34:02 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/08 18:57:48 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Configuration.hpp>
#include <config/exceptions/ConfigurationBindException.hpp>
#include <config/exceptions/ConfigurationValidateException.hpp>
#include <exception/IOException.hpp>
#include <util/ContainerBuilder.hpp>
#include <util/Enum.hpp>
#include <util/json/JsonException.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/log/LogLevel.hpp>
#include <util/options/CommandLine.hpp>
#include <util/options/Option.hpp>
#include <util/options/OptionParser.hpp>
#include <iostream>
#include <list>
#include <string>
#include <typeinfo>
#include <vector>

const Option OPT_HELP('h', "help", "display this help message");
const Option OPT_LOG_LEVEL('l', "log-level", "change the log-level", "level");
const Option OPT_CHECK('c', "check", "only check the config file");
const Option OPT_CONFIG_FILE('f', "config-file", "specify the config file", "file");

int
delegated_main(int argc, char **argv)
{
	const char *program = argv[0];

	std::string configFile = "conf.json";
	bool checkOnly = false;
	const LogLevel *level = LogLevel::INFO;

	std::list<const Option*> lst;
	lst.push_back(&OPT_HELP);
	lst.push_back(&OPT_LOG_LEVEL);
	lst.push_back(&OPT_CHECK);
	lst.push_back(&OPT_CONFIG_FILE);

	OptionParser parser(lst);

	try
	{
		CommandLine commandLine = parser.parse(argc, argv);

		if (commandLine.has(OPT_HELP))
		{
			std::vector<std::string> authors = ContainerBuilder<std::string>()
			/**/.add("Enzo CACERES <ecaceres@student.42.fr>")
			/**/.add("Alice TETU <alicetetu@student.42.fr>")
			/**/.build();

			std::cout << parser.help(program, "A small web server", authors) << std::endl;
			return (0);
		}

		if (commandLine.has(OPT_LOG_LEVEL))
		{
			const std::string &input = commandLine.last(OPT_LOG_LEVEL);

			const LogLevel *foundLevel = LogLevel::find(input);
			if (foundLevel)
				level = foundLevel;
			else
				throw Exception("unknown log level: " + input);
		}

		if (commandLine.has(OPT_CHECK))
		{
			checkOnly = true;
		}

		if (commandLine.has(OPT_CONFIG_FILE))
			configFile = commandLine.last(OPT_CONFIG_FILE);
	}
	catch (Exception &exception)
	{
		std::cerr << argv[0] << ": " << exception.what() << std::endl;
		std::cerr << "Try '" << argv[0] << " --help' for more informations." << std::endl;
		return (1);
	}

	LogLevel::ACTIVE = level;
	Logger &LOG = LoggerFactory::get("main");

	LOG.debug() << "Set log level to: " << level->name() << std::endl;

	Configuration *configuration = NULL;

	try
	{
		LOG.debug() << "Loading configuration... (path: " << configFile << ")" << std::endl;

		configuration = Configuration::fromJsonFile(configFile);
	}
	catch (IOException &exception)
	{
		LOG.fatal() << "Failed to read file: " << exception.message() << std::endl;
		return (1);
	}
	catch (JsonException &exception)
	{
		LOG.fatal() << "Failed to parse JSON: " << exception.message() << std::endl;
		return (1);
	}
	catch (ConfigurationBindException &exception)
	{
		LOG.fatal() << "Failed bind configuration: " << exception.message() << std::endl;
		return (1);
	}
	catch (ConfigurationValidateException &exception)
	{
		LOG.fatal() << "Failed validate configuration: " << exception.message() << std::endl;
		return (1);
	}
	catch (Exception &exception)
	{
		LOG.fatal() << "Failed create configuration: " << exception.message() << std::endl;
		return (1);
	}

	delete configuration;
	std::cout << configuration << std::endl;

	if (checkOnly)
		return (0);

	// TODO Start orchestrator

	return (0);
}

int
main(int argc, char **argv)
{
	int exitCode;

	try
	{
		exitCode = delegated_main(argc, argv);
	}
	catch (std::exception &exception)
	{
		exitCode = 1;

		std::cerr << "unhandled " << typeid(exception).name() << ": " << exception.what() << std::endl;
	}

	LoggerFactory::destroy();

	return (exitCode);
}
