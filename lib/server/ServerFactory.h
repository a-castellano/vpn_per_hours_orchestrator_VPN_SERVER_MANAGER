// ServerFactory.h 13/03/2016
// Álvaro Castellano Vela

#ifndef SERVERFACTORY_H
#define SERVERFACTORY_H

#include <string>
#include <map>
#include "Server.h"
#include "DigitalOceanServer.h"
#include "VultrServer.h"



Server *CreateServer(const std::string &,const std::string &);


#endif
