#include "OptionParser.hpp"
#include "Option.hpp"
#include <list>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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

int file_exists(std::string configFile)
{
	char buffer[512];
	struct stat	stat_buff[3];
	int is_child_file;
	char 

	::memset(buffer, 0, 512); // pas le droit a memset
	if (!::getcwd(buffer, 512))
		::exit (1);
	std::string current = buffer;
	std::string file = current + "/" + configFile;
//	std::cout << file << std::endl;
	if (configFile.c_str()[0] == '.' && configFile.c_str()[0] == '/')
	{
		is_child_file = 1;
		strd::string child_file = 
	}
	if (::stat(file.c_str(), &stat_buff[0])
		|| ::stat(configFile.c_str(), &stat_buff[1]))
		return (0);
	return (1);
}

int main(int argc, char **argv)
{
	std::string helpMessage[3]= {
	"-h	--help		: no value : display this help message",
 	"-c	--check		: no value : only check the config file",
 	"-f	--config file	: <file>   : specify the config file"
	};

	Option helpOption("-h", "--help", "display this help message", false, "", "");
	Option checkOption("-c", "--check", "only check the config file", false, "", "");
	Option configFileOption("-f", "--config-file", "specify the config file", false, "", "");
 
	std::string configFile;
// c (...)
 // c (..., valueName)

	std::list<Option*> lst;
	lst.push_back(&helpOption);
 	lst.push_back(&checkOption);
	lst.push_back(&configFileOption);
 	OptionParser parser(lst);
	
	try 
	{
		parser.parse(argc, argv);
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what();
		::exit(1);
	}
	if (helpOption.hasValue())
	{
		for (int i = 0; i < 3; i++)
			std::cout << helpMessage[i] << std::endl;
	}
	if (configFileOption.hasValue())
		configFile = configFileOption.getValue();
	if (!(file_exists(configFile)))
	{
		std::cerr << "Config file not found" << std::endl;
  		::exit(1);
	}
 }
 /* 
 * std::string configFile = "./conf.json";
 * 
 if(parser.has(configFileOption)) {
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
