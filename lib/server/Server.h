// Server.h
// Álvaro Castellano Vela 11/03/2016

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <iostream>
#include "api_keys.h"

class Server
{
	public:
		Server( const std::string & );

		virtual bool create() = 0;
		virtual bool destroy() = 0;
		virtual bool powerOn() = 0;
		virtual bool powerOff() = 0;

		virtual const std::string serverType();
		const std::string getToken();
		const unsigned int getZone();
		void setZone(const unsigned int &);
		
		void setTrueZone(const std::string &);
		std::string  getTrueZone();

		void setMachineID(const std::string &);
		std::string getMachineID();

		void setServerIP(const std::string &);
		std::string getServerIP();

		void setServerName(const std::string &);
		std::string getServerName(); 

		static size_t data_write(void*, size_t , size_t ,void*);//this

		void setVpnGroups(const std::vector<std::string> &);

		//this function should be private
		~Server();

	private:
		std::string token;
		std::string state;
		unsigned int zone;
		std::string realZone;
		std::string machineID;
		std::string serverIP;
		std::string serverName;
		std::vector<std::string> vpnGroups;
};

#endif
