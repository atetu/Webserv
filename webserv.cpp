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

// #include <config/block/LocationBlock.hpp>
// #include <config/block/ServerBlock.hpp>
#include "config/Configuration.hpp"
// #include <exception/Exception.hpp>
// #include <http/HTTPMethod.hpp>
#include <http/HTTPOrchestrator.hpp>
// #include <util/ContainerBuilder.hpp>
// #include <util/log/LoggerFactory.hpp>
// #include <util/unit/DataSize.hpp>
#include <util/options/Option.hpp>
#include <util/options/OptionParser.hpp>
#include <util/options/ParserException.hpp>
#include <util/json/Json.hpp>
#include <csignal>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(int argc, char **argv)
{

	//OPTIONS

	Option helpOption("-", "--", 'h', "help", "display this help message", false, "", "");
	Option checkOption("-", "--",'c', "check", "only check the config file", false, "", "");
	Option configFileOption("-", "--",'f', "config-file", "specify the config file", true, "", "");
 
	std::string configFile;

	std::list<Option*> lst;
	lst.push_back(&helpOption);
 	lst.push_back(&checkOption);
	lst.push_back(&configFileOption);
 	std::map<char, std::string> optionMap;

	OptionParser parser(lst, &optionMap);
	
	try 
	{
		parser.parse(argc, argv);
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
		::exit(1);
	}
	if (optionMap.find('h') != optionMap.end())
	{
		for (std::list<Option*>::iterator it = lst.begin(); it!= lst.end(); ++it)
		{
			std::cout << (*it)->getShortStart() << (*it)->getShortArg() << "	"
				<< (*it)->getLongStart() << (*it)->getLongArg();
			if ((*it)->hasValue())
				std::cout << "	";
			else
				std::cout << "		";
			std::cout << (*it)->getDescription() << std::endl;
		}
	}
	if (configFileOption.getValue().compare("") != 0)
	{
		configFile = configFileOption.getValue();
		std::cout << "config file: " << configFile << std::endl;
	}
	else
		configFile = "conf.json";
	
	struct stat buf;
	if (stat(configFile.c_str(), &buf) != 0)
	{
		if (optionMap.find('c') != optionMap.end() || optionMap.find('f') != optionMap.end() || optionMap.find('h') == optionMap.end())
		{
			std::cerr << "Config file does not exist.\n";
			::exit(1);
		}
	}
	if (optionMap.find('c') != optionMap.end())
	{
		// CHECK CONF FILE
	}
	
	Configuration configuration;
	try
	{
		configuration = Configuration::fromJsonFile(configFile);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		::exit(2);
	}
	

	
// 	Configuration configuration(ContainerBuilder<ServerBlock>() //
// 	/**/.add(ServerBlock()
// 	/**//**/.port(1502)
// 	/**//**/.host("127.0.0.1")
// 	/**//**/.name("boxplay.io")
// 	/**//**/.maxBodySize(DataSize::ofMegabytes(10))
// 	/**//**/.root("/var/www/html")
// 	/**//**/.locations(ContainerBuilder<LocationBlock>()
// 	/**//**//**/.add(LocationBlock()
// 	/**//**//**//**/.methods(ContainerBuilder<std::string>()
// 	/**//**//**//**//**/.add("GET")
// 	/**//**//**//**//**/.add("HEAD")
// 	/**//**//**//**//**/.add("POST")
// 	/**//**//**//**//**/.build())
// 	/**//**//**//**/.index(ContainerBuilder<std::string>()
// 	/**//**//**//**//**/.add("index.html")
// 	/**//**//**//**//**/.add("index-default.html")
// 	/**//**//**//**//**/.build())
// 	/**//**//**//**/.listing(true))
// 	/**//**//**/.build()))
// 	/**/.add(ServerBlock()
// 	/**//**/.port(2051)
// 	/**//**/.host("127.0.0.1")
// 	/**//**/.name("boxplay2.io")
// 	/**//**/.maxBodySize(DataSize::ofMegabytes(10))
// 	/**//**/.root("/var/www/html"))
// 	/**/.build());

// 	signal(SIGPIPE, SIG_IGN);

 	try {
 		HTTPOrchestrator::create(configuration).start();
 	}
	catch (Exception &e) {
 		std::cerr << "Ouch... The server has thrown an exception: " << std::endl;
 		std::cerr << typeid(e).name() << ".what(): " << e.what() << std::endl;
 	}

// //	throw IOException("ORCHESTRATOR LOOP HAS BEEN EXITED", errno);

// 	LoggerFactory::destroy();

	return (0);
}
