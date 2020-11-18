

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
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
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        std::cout <<"\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
		char buffer_file[30000] = {0};
		int fd;
		std::string file;
        valread = read( new_socket , buffer, 30000);
		file = parse(buffer);
        fd = open(file.c_str(), O_RDONLY);
     	read (fd, buffer_file, 30000);
       
       	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
     	std::string length_str = std::to_string(std::string(buffer_file).size());
		std::string blank_line = "\n\n";
		std::string str_to_send = header + length_str + blank_line + std::string(buffer_file);
		
		write(new_socket , str_to_send.c_str() , strlen(str_to_send.c_str()));
       
        std::cout << "------------------Hello message sent-------------------\n";
       
        close(new_socket);
    }
    return 0;
}