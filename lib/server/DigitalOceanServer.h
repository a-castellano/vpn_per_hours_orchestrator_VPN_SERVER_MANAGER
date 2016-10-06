// DigitalOceanServer.h
// Álvaro Castellano Vela 11/03/2016

#ifndef DIGITALOCEANSERVER_H
#define DIGITALOCEANSERVER_H

#include "Server.h"
#include <string>

class DigitalOceanServer : public Server
{
        public:
                DigitalOceanServer( const std::string & );

		bool create();
		bool destroy();
		bool powerOn();
		bool powerOff();

		const std::string serverType();

		static Server * Create(const std::string &token) { return new DigitalOceanServer(token); }

		~DigitalOceanServer();


};

#endif
