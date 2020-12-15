#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <exception/IOException.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPMethod.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPRequestParser.hpp>
#include <http/HTTPValueParser.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <io/SocketServer.hpp>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <util/buffer/BaseBuffer.hpp>
#include <util/buffer/IOBuffer.hpp>
#include <util/Enum.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/System.hpp>
#include <util/URL.hpp>
#include <set>
#include <utility>


#include <http/handler/HTTPMethodHandler.hpp>
#include <http/handler/methods/GetHandler.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPHeaderParser.hpp>

class System;

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


void test(int n, const char *str, HTTPMethod *method, const std::string &path, HTTPVersion *version, HTTPHeaderFields &fields)
{
    HTTPRequestParser parser;
    
    while (*str && parser.state() != HTTPRequestParser::S_END && parser.state() != HTTPRequestParser::S_CONTINUE)
    {
  //      std::cout << *str << std::endl;
         parser.consume(*str);
         str++;
    }
   
   
    if (parser.state() == HTTPRequestParser::S_CONTINUE)
    {
        char c = parser.lastChar();
        while (1)
        {
            HTTPHeaderParser headerParser;
            headerParser.consume(c);
       
            while (*str)
            {
                
                headerParser.consume(*str);
                if (headerParser.state() == HTTPHeaderParser::S_END || headerParser.state() == HTTPHeaderParser::S_CONTINUE)
                    break;
                str++;
            }
            parser.header(headerParser);
            if (headerParser.state() == HTTPHeaderParser::S_END)
                break;
        }
        
    }	
 
     HTTPHeaderFields *header = HTTPHeaderFields::create(parser.getHeader());				  
    if (parser.method() != method->name())
        throw Exception("Mismatch, expected method: " + method->name() + " but got " + parser.method());
   
    if (parser.path() != path)
        throw Exception("Mismatch, expected path: " + path + " but got " + parser.path());
    
//    if (header->storage() != fields.storage())
//     {
//        throw Exception("Mismatch regarding the header fields");
        // std::map<std::string, std::string>::iterator it = header->storage().begin();
        // std::map<std::string, std::string>::iterator ite = header->storage().end();

        // while (it != ite)
        // {
        //    std::cout << it->first << " : " << it->second << std::endl;
        //     it++;
        // }
    
       
    // }
  //  if (header->getValueWithWeight("Accept-Charsets") == fields.getValueWithWeight("Accept-Charsets"))
    //    std::cout << "value parser ok \n";
     std::map<std::string, std::string> map;
     map = fields.storage();
    std::map<std::string, std::string>::iterator it_map = map.begin();
     std::map<std::string, std::string>::iterator ite_map = map.end();
    //  while (it_map != ite_map)
    //  {
    //      std::cout << it_map->first << std::endl;
    //     if (!(header->getValueWithWeight(it_map->first) == fields.getValueWithWeight(it_map->first)))
    //         throw Exception("Mismatch regarding the value with weight");
    //     it_map++;
    // }
    while (it_map != ite_map)
    {
    std::cout << "map first : " << it_map->first << std::endl;
    HTTPValueParser *valueParser = header->getValueWithWeight(it_map->first);
    // if ((header->getValueWithWeight(it_map->first) == fields.getValueWithWeight(it_map->first)))
    //     std::cout << "accept equal\n";
    std::map<float,  std::list<std::string> > *parsed_map = valueParser->getMap();
    std::map<float,  std::list<std::string> >::iterator itt =parsed_map->begin();
    std::map<float,  std::list<std::string> >::iterator itte = parsed_map->end();
    
    HTTPValueParser *valueParser2 = fields.getValueWithWeight(it_map->first);
    std::map<float,  std::list<std::string> > *parsed_map2 = valueParser2->getMap();
    std::map<float,  std::list<std::string> >::iterator itt2 =parsed_map2->begin();
    std::map<float,  std::list<std::string> >::iterator itte2= parsed_map2->end();

    while (itt != itte && itt2 != itte2)
    {
        if (*itt != *itt2)
            throw Exception("Pair not equal");
        itt++;
        itt2++;
        // std::cout << itt->first << " : " ;
        
        // std::list<std::string>::iterator it_list = itt->second.begin();
        
        // std::list<std::string>::iterator ite_list = itt->second.end();
    
        // while (it_list != ite_list)
        // {
            
        //         std::cout<< *it_list << std::endl;
        //         it_list++;
        // }
        // itt++;
    }
    if (itt != itte || itt2 != itte2)
        throw Exception("Pair not equal");

    // HTTPValueParser *valueParser2 = fields.getValueWithWeight(it_map->first);
    // std::map<float,  std::list<std::string> > *parsed_map2 = valueParser2->getMap();
    // std::map<float,  std::list<std::string> >::iterator itt2 =parsed_map2->begin();
    // std::map<float,  std::list<std::string> >::iterator itte2= parsed_map2->end();
    
    // while (itt2 != itte2)
    // {
    //     std::cout << itt2->first << " : " ;
        
    //     std::list<std::string>::iterator it_list = itt2->second.begin();
        
    //     std::list<std::string>::iterator ite_list = itt2->second.end();
    
    //     while (it_list != ite_list)
    //     {
            
    //             std::cout<< *it_list << std::endl;
    //             it_list++;
    //     }
    //     itt2++;
    // }
   it_map++;
    }
    std::cout << n << " : test ok\n";
}

int main()
{
    HTTPVersion correctHttpVersion(1, 1);

    int n = 0;
    try
    {
        n++;
        test(n, "GET / HTTP/1.1\r\n Accept-Charsets:   iso;q=0.2, vhnbj\r\n\r\n",
        HTTPMethod::GET,
         "/",
         &correctHttpVersion,
         HTTPHeaderFields().set("Accept-Charsets", "iso;q=0.2"));
    
    // test("GET / HTTP/1.1\r\n"
    //      "Hello: World\r\n"
    //      "\r\n",
    //      HTTPMethod::GET,
    //      "/",
    //      HTTPVersion::HHTP_1_1,
    //      HTTPHeaderFields().set("Hello", "World"));
    
    // test("GET / HTTP/1.1\r\n"
    //      "Hello: World\r\n"
    //      "\r\n",
    //      HTTPMethod::GET,
    //      "/",
    //      HTTPVersion::HHTP_1_1,
    //      HTTPHeaderFields().set("Hello", "World"));
    }
    catch(const std::exception& e)
    {
        std::cerr << n << " : " << e.what() << '\n';
    }


    try
    {
        n++;
        test(n, "GET / HTTP/1.1\r\n Accept-Charsets:   iso;q=0.2, vhnbj\r\n\r\n",
        HTTPMethod::GET,
         "/",
         &correctHttpVersion,
         HTTPHeaderFields().set("Accept-Charsets", "   iso;q=0.2, vhnbj"));
    }
    catch(const std::exception& e)
    {
        std::cerr << n << " : " << e.what() << '\n';
    }

    try
    {
        n++;
        test(n, "GET / HTTP/1.1\r\n"
        "Accept-Charsets:   iso;q=0.2, vhnbj\r\n"
        "Accept-Language:   en,fr;q=0.2, sp\r\n"
        "\r\n",
        HTTPMethod::GET,
         "/",
         &correctHttpVersion,
         HTTPHeaderFields().set("Accept-Charsets", "iso;q=0.2, vhnbj"). set("Accept-Language", "en, fr;q=0.2, sp"));
    }
    catch(const std::exception& e)
    {
        std::cerr << n << " : " << e.what() << '\n';
    }
}