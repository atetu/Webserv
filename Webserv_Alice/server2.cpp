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
#include <arpa/inet.h>
#define PORT 8080

#define SOCKET int
#define INVALID_SOCKET ((int)-1)
#define SOCKET_ERROR (int(-1))

typedef struct t_client {
	    int sckt;
	    sockaddr_in addr;
    } Client;

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

std::string GetAddress(const sockaddr_in& addr)
	{
		char buff[INET6_ADDRSTRLEN] = { 0 };
		return (inet_ntop(addr.sin_family, (void*)&(addr.sin_addr), buff, INET6_ADDRSTRLEN));
	}

int main(int argc, char const *argv[])
{
    int server_fd; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    

 //   std::vector<Client> clients;

   
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

	std::vector<Client> clients;
	for (;;)
	{
		{
			fd_set set;
			timeval timeout = { 0 };
			FD_ZERO(&set);
			FD_SET(server_fd, &set);
			int selectReady = select(static_cast<int>(server_fd) + 1, &set, nullptr, nullptr, &timeout);
			if (selectReady == -1)
			{
				std::cout << "Erreur select pour accept : " << std::endl;
				break;
			}
			if (selectReady > 0)
			{
				sockaddr_in from = { 0 };
				socklen_t addrlen = sizeof(from);
				int newClientSocket = accept(server_fd, (struct sockaddr *)(&from), &addrlen);
				if (newClientSocket != -1)
				{
					//if (!Sockets::SetNonBlocking(newClientSocket))
					//{
					//	std::cout << "Erreur settings nouveau socket non-bloquant : " << Sockets::GetError() << std::endl;
					//	Sockets::CloseSocket(newClientSocket);
					//	continue;
					//}
					Client newClient;
					newClient.sckt = newClientSocket;
					newClient.addr = from;
					const std::string clientAddress = GetAddress(from);
					const unsigned short clientPort = ntohs(from.sin_port);
					std::cout << "\n\nConnexion de " << clientAddress.c_str() << ":" << clientPort << std::endl;
					clients.push_back(newClient);
				}
			}
		}
		if (!clients.empty())
		{
			fd_set setReads;
			fd_set setWrite;
			fd_set setErrors;
			FD_ZERO(&setReads);
			FD_ZERO(&setWrite);
			FD_ZERO(&setErrors);
			int highestFd = 0;
			timeval timeout = { 0 };
	
	
			for (std::vector<Client>::iterator client = clients.begin(); client!= clients.end(); ++client)
			{
				FD_SET(static_cast<int>((*client).sckt), &setReads);
				FD_SET(static_cast<int>((*client).sckt), &setWrite);
				FD_SET(static_cast<int>((*client).sckt), &setErrors);
				if ((*client).sckt > highestFd)
					highestFd = static_cast<int>((*client).sckt);
			}
			int selectResult = select(highestFd + 1, &setReads, &setWrite, &setErrors, &timeout);
			if (selectResult == -1)
			{
				std::cout << "Erreur select pour clients : " << std::endl;
				break;
			}
			else if (selectResult > 0)
			{
				std::vector<Client>::iterator itClient = clients.begin();
				while (itClient != clients.end())
				{
					const std::string clientAddress = GetAddress(itClient->addr);
					const unsigned short clientPort = ntohs(itClient->addr.sin_port);

					bool hasError = false;
					if (FD_ISSET(itClient->sckt, &setErrors))
					{
						std::cout << "Erreur" << std::endl;
						hasError = true;
					}
					else if (FD_ISSET(itClient->sckt, &setReads))
					{
						
						char buffer[30000] = {0};
						char buffer_file[300] = {0};
						int fd;
						std::string file;
						valread = read( itClient->sckt , buffer, 30000);
						file = parse(buffer);

						struct stat sb;
				
						if (stat(file.c_str(), &sb) == -1)
						{
							perror("stat");
						//   exit(EXIT_FAILURE);
						}
						long long file_size = sb.st_size;
						fd = open(file.c_str(), O_RDONLY);
						
						std::string header = "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: ";
						std::string length_str = std::to_string(file_size);
						std::cout << length_str;
						std::string blank_line = "\r\n\r\n";
					
						if (file.compare("buse.png") == 0)
						{
						std::string str_to_send = header + length_str + blank_line;
						std::cout << "\n\n[" << clientAddress << ":" << clientPort << "]" << buffer << std::endl;
						std::cout<< "\n\n|||||||||||||||\n\n";
						if (FD_ISSET(itClient->sckt, &setWrite))
						{
							write(itClient->sckt , str_to_send.c_str() , str_to_send.length());
				
							int ret;
							while ((ret = (read(fd, buffer_file, 300))))
								write(itClient->sckt, buffer_file, ret);
						}
						}
						else
						{
							std::string error_header = "HTTP/1.1 404 Not Found";
							if (FD_ISSET(itClient->sckt, &setWrite))
								write(itClient->sckt ,error_header.c_str() , error_header.length());
						}
						
       



						// char buffer[200] = { 0 };
						// int ret = recv(itClient->sckt, buffer, 199, 0);
						// if (ret == 0)
						// {
						// 	std::cout << "Connexion terminee" << std::endl;
						// 	hasError = true;
						// }
						// else if(ret == SOCKET_ERROR)
						// {
						// 	std::cout << "Erreur reception : " <<  std::endl;
						// 	hasError = true;
						// }
						// else
						// {
						// 	std::cout << "\n\n[" << clientAddress << ":" << clientPort << "]" << buffer << std::endl;
						// 	if (FD_ISSET(itClient, &setWrite))
						// 	{
						// 		std::cout << "ici\n";
						// 		ret = send(itClient->sckt, buffer, ret, 0);
						// 		if (ret == 0 || ret == SOCKET_ERROR)
						// 		{
						// 			std::cout << "Erreur envoi" <<  std::endl;
						// 			hasError = true;
						// 		}
						// 	}
						// }
					}
					if (hasError)
					{
						//!< DÈconnectÈ
						std::cout << "Deconnexion de [" << clientAddress << ":" << clientPort << "]" << std::endl;
						itClient = clients.erase(itClient);
					}
					else
					{
						++itClient;
					}
				}
			}
		}
	}
///	Sockets::CloseSocket(server);
//	Sockets::Release();
std::cout << "\n\n||||||||||||||||||||||||\n\n";
close(server_fd);
	return 0;
}

