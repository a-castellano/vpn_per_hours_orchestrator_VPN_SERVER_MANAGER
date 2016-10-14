// ServerFactory.h 13/03/2016
// Álvaro Castellano Vela

#ifndef SERVERFACTORY_H
#define SERVERFACTORY_H

#include <string>
#include <map>
#include "Server.h"
#include "DigitalOceanServer.h"
#include "VultrServer.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>


Server *CreateServer(const std::string &, boost::shared_ptr<std::string> );


#endif
