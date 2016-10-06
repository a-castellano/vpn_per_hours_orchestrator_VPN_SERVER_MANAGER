// ServerPointer.cpp
// Álvaro Castellano Vela - 25/04/2016

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ServerPointer.h"

ServerPointer::ServerPointer(const std::string &server, const int &port)
    : server(server), port(port) {}

bool ServerPointer::call(const std::string &msg, std::string &stringresponse) {
  int sockfd;
  int n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char response[256];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "ERROR opening socket" << std::endl;
    return false;
  }

  server = gethostbyname(this->server.c_str());
  if (server == NULL) {
    std::cerr << "ERROR, no such host" << std::endl;
    return false;
  }

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(port);
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "ERROR connecting" << std::endl;
    return false;
  }
  n = write(sockfd, msg.c_str(), msg.size());
  if (n < 0) {
    std::cerr << "ERROR writing to socket" << std::endl;
    return false;
  }
  bzero(response, 256);
  n = read(sockfd, response, 255);
  if (n < 0) {
    std::cerr << "ERROR reading from socket" << std::endl;
    return false;
  }
	stringresponse = std::string(response);
  close(sockfd);

  return true;
}

bool ServerPointer::point(const std::string &subdomain, const std::string &ip) {
	std::string response;

  std::string separator = "|##|";
  std::string msg = subdomain + separator + ip + separator + std::string("ADD");

	if ( ! this->call(msg, response) )
	{
		return false;
	}

  std::cout << response << std::endl;


  return true;
}

bool ServerPointer::unpoint(const std::string &subdomain) {

	std::string response;
	std::string separator = "|##|";
	std::string msg = subdomain + separator + std::string("NOMATTER") + separator + std::string("DELETE");

	if ( ! this->call(msg, response) )  
	{
		return false;
	}

	std::cout << response << std::endl;  

	return true;
}
