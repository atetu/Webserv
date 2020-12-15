

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#define PORT 8080


std::string parse(char buffer[])
{
	int i;
	int start;
	int end;

	i = 0;
	while (buffer[i] != '/')
		i++;
    i++;
	start = i;
	while (buffer[i] != ' ')
		i++;
	end = i -1;
	std::string file = (std::string(buffer)).substr(start, (end - start +1));
    return (file);
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    struct Client {
	    int sckt;
	    sockaddr_in addr;
    };

    std::vector<Client> clients;

   
//  char *hello = "Hello from server";
//   char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        close (server_fd);
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {close (server_fd);

        perror("In listen");
        exit(EXIT_FAILURE);
    }
   
    // fd_set set;
    // timeval timeout = { 0 };
    // FD_ZERO(&set);
    // FD_SET(server, &set);
    // int selectReady = select(server_fd + 1, &set, nullptr, nullptr, &timeout);
    // if (selectReady == -1)
    // {
    //     std::cout << "Erreur select pour accept : " << Sockets::GetError() << std::endl;
    //     break;
    // }
    // else if (selectReady > 0) 
    // {
    //     // notre socket server est prêt à être lu
    //     sockaddr_in from = { 0 };
    //     socklen_t addrlen = sizeof(from);
    //     int newClientSocket = accept(server_fd, (struct sockaddr *)(&from), &addrlen);
    //     if (newClientSocket != -1)
    //     {
    //         Client newClient;
    //         newClient.sckt = newClientSocket;
    //         newClient.addr = from;
    //         const std::string clientAddress = Sockets::GetAddress(from);
    //         const unsigned short clientPort = ntohs(from.sin_port);
    //         std::cout << "Connexion de " << clientAddress.c_str() << ":" << clientPort << std::endl;
    //     }
    // }
  
  
    while(1)
    {
        std::cout <<"\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            close (server_fd);
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
		char buffer_file[300] = {0};
		int fd;
		std::string file;
        valread = read( new_socket , buffer, 30000);
		file = parse(buffer);

        struct stat sb;
  
        if (stat(file.c_str(), &sb) == -1)
        {
            perror("stat");
         //   exit(EXIT_FAILURE);
        }

        
      long long file_size = sb.st_size;
       
        
     fd = open(file.c_str(), O_RDONLY);
      


//MARCHE
// FILE * pFile;
//   long lSize;
//   char * buf;
//   size_t result;

//   pFile = fopen ( file.c_str(), "rb" );
//   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

//      fseek (pFile , 0 , SEEK_END);
//   lSize = ftell (pFile);
//   rewind (pFile);

//   buf = (char*) malloc (sizeof(char)*lSize);
//   if (buf == NULL) {fputs ("Memory error",stderr); exit (2);}

//   result = fread (buf,1,lSize,pFile);
//   if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

   
       
     	std::string header = "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: ";
        std::string length_str = std::to_string(file_size);
         std::cout << length_str;
		std::string blank_line = "\r\n\r\n";
      
		std::string str_to_send = header + length_str + blank_line;
	
	write(new_socket , str_to_send.c_str() , str_to_send.length());
  
        int ret;
        while ((ret = (read(fd, buffer_file, 300))))
            write(new_socket , buffer_file, ret);
       
        std::cout << "------------------Hello message sent-------------------\n";
       



     //   close(new_socket);
         // terminate
  //close (fd);
 // free (buf);



//     }
  
//     return 0;
// }