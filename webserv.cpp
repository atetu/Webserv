/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 15:34:02 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/19 15:34:02 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/block/LocationBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/Configuration.hpp>
#include <exception/Exception.hpp>
#include <http/HTTPMethod.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <util/ContainerBuilder.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/unit/DataSize.hpp>
#include <csignal>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

int
main(int argc, char **argv)
{
	Configuration configuration(ContainerBuilder<ServerBlock>() //
	/**/.add(ServerBlock()
	/**//**/.port(1502)
	/**//**/.host("127.0.0.1")
	/**//**/.name("boxplay.io")
	/**//**/.maxBodySize(DataSize::ofMegabytes(10))
	/**//**/.root("/var/www/html")
	/**//**/.locations(ContainerBuilder<LocationBlock>()
	/**//**//**/.add(LocationBlock()
	/**//**//**//**/.methods(ContainerBuilder<std::string>()
	/**//**//**//**//**/.add("GET")
	/**//**//**//**//**/.add("HEAD")
	/**//**//**//**//**/.add("POST")
	/**//**//**//**//**/.build())
	/**//**//**//**/.index(ContainerBuilder<std::string>()
	/**//**//**//**//**/.add("index.html")
	/**//**//**//**//**/.add("index-default.html")
	/**//**//**//**//**/.build())
	/**//**//**//**/.listing(true))
	/**//**//**/.build()))
	/**/.add(ServerBlock()
	/**//**/.port(2051)
	/**//**/.host("127.0.0.1")
	/**//**/.name("boxplay2.io")
	/**//**/.maxBodySize(DataSize::ofMegabytes(10))
	/**//**/.root("/var/www/html"))
	/**/.build());

	signal(SIGPIPE, SIG_IGN);

	try {
		HTTPOrchestrator::create(configuration).start();
	} catch (Exception &e) {
		std::cerr << "Ouch... The server has thrown an exception: " << std::endl;
		std::cerr << typeid(e).name() << ".what(): " << e.what() << std::endl;
	}

//	throw IOException("ORCHESTRATOR LOOP HAS BEEN EXITED", errno);

	LoggerFactory::destroy();

	return (0);
}
