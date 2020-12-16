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
#include <http/HTTPFindLocation.hpp>
#include <http/HTTPLocationInterpretor.hpp>

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



int main(int argc, char **argv)
{
    HTTPVersion correctHttpVersion(1, 1);
    Configuration configuration;
    configuration = Configuration::fromJsonFile(argv[1]);
    configuration.rootBlock().serverBlocks().get();
    //.rootBlock()->serverBlocks();
    std::list<const ServerBlock*> serverList = configuration.rootBlock().serverBlocks().get();
    std::list<const ServerBlock*>::iterator it = serverList.begin();
    std::list<const ServerBlock*>::iterator ite = serverList.end();
    std::string path = "/";
    const LocationBlock *locationBlock;
   
    while(it != ite)
    { 
        const ServerBlock *server = *it;
        std::list<const LocationBlock*> locationList = server->locations().get();
        std::list<const LocationBlock*>::iterator loc_it = locationList.begin();
         std::list<const LocationBlock*>::iterator loc_ite = locationList.end();
         while (loc_it != loc_ite)
         {
             std::cout << (*loc_it)->path() << std::endl;
             loc_it++;
         }
        HTTPFindLocation findLocation(path, locationList);
        findLocation.parse();
         
        if (findLocation.location().present())
        {
            locationBlock = findLocation.location().get();
            break;
        }
        it++;
    }
    locationBlock->path();
    // if (locationBlock->path()!= "")
    //     std::cout << locationBlock->path() << std::endl;
//    delete locationBlock;
    //std::cout << locationBlock->path << std::endl; // does not work

   
}