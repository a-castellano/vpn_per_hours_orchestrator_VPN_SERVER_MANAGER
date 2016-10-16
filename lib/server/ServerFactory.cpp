// ServerFactory.cpp
// Álvaro Castellano Vela 13/03/2016

#include "ServerFactory.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

Server *CreateServer(const std::string &provider,
                     boost::shared_ptr<std::string> token) {
  if (provider == "DigitalOcean")
    return new DigitalOceanServer(token);
  if (provider == "Vultr")
    return new VultrServer(token);
  return NULL;
}
