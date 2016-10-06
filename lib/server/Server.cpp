// Server.cpp
// Álvaro Castellano Vela 11/03/2016

#include "Server.h"

Server::Server(const std::string &token)
:token(token)
{}

const std::string Server::getToken( void )
{
	return token;
}

const unsigned int Server::getZone( void )
{
return zone;
}

const std::string Server::serverType( void )
{
        return std::string("None");
}

void Server::setZone(const unsigned int & zone)
{
	this->zone = zone;
}

size_t Server::data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if(userp)
			{
						std::ostream& os = *static_cast<std::ostream*>(userp);
								std::streamsize len = size * nmemb;
										if(os.write(static_cast<char*>(buf), len))
														return len;
											}

		return 0;
}

void Server::setMachineID(const std::string &id)
{
	machineID = id;
}

std::string Server::getMachineID()
{
	return machineID;
}

void Server::setServerIP(const std::string &ip)
{
	serverIP = ip;
}

std::string Server::getServerIP()
{
	return serverIP;
}

void Server::setServerName(const std::string &name)
{
	serverName = name;
}
std::string Server::getServerName()
{
	return serverName;
}

void Server::setTrueZone(const std::string &zone)
{
	realZone = zone;
}

std::string Server::getTrueZone()
{
	return realZone;
}

Server::~Server() {}

