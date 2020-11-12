#include "OptionParser.hpp"
#include "Option.hpp"

// ./webserv --check --config-file ./x.json


/**
 * webserv v01
 * 
 * -h	--help			: no value : display this help message
 * -c	--check			: no value : only check the config file
 * -f	--config file	: <file>   : specify the config file
 * 
 * 
 * */

 int main(int argc, char **argv)
 {
	 
	Option helpOption('h', "help", "display this help message", false, "", "");
	Option checkOption('c', "check", "only check the config file", false, "", "");
	Option configFileOption('f', "config-file", "specify the config file", false, "", "");
 
// c (...)
 // c (..., valueName)
 
 	OptionParser parser(&helpOption, &checkOption, &configFileOption);
	
	try 
	{
		parser.parse(argc, argv);
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what();
		::exit(1);
	}
 }
 /* 
 * std::string configFile = "./conf.json";
 * 
 * if(parser.has(configFileOption)) {
 * 		configFile = configFileOption.value();
 * }
 * 
 * if (file does not exists (configFile)) {
 * 		std::cerr << config file not found,
 * 		exit(1);
 * }
 * 
 * if (parser.isPresent(checkOption)) {
 * 		// CHECK CONFIG FILE
 * 		::exit(0);
 * }
 * 
 * // Bind JSON to CPP
 * // Start orchestrator
 * 
 */
