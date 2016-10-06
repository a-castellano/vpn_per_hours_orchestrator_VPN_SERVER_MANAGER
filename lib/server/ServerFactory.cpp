// ServerFactory.cpp
// Álvaro Castellano Vela 13/03/2016

#include "ServerFactory.h"

Server *CreateServer(const std::string &provider, const std::string &token) {
  if( provider == "DigitalOcean")
    return new DigitalOceanServer(token);
  if( provider == "Vultr")
    return new VultrServer(token);
return NULL;
}
