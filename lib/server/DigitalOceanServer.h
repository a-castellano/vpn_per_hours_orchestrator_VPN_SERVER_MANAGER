// DigitalOceanServer.h
// Álvaro Castellano Vela 11/03/2016

#ifndef DIGITALOCEANSERVER_H
#define DIGITALOCEANSERVER_H

#include "Server.h"
#include <string>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

class DigitalOceanServer : public Server {
public:
  DigitalOceanServer(boost::shared_ptr<std::string>);

  bool create();
  bool destroy();
  bool powerOn();
  bool powerOff();

  const std::string serverType();

  static Server *Create(boost::shared_ptr<std::string> token) {
    return new DigitalOceanServer(token);
  }

  ~DigitalOceanServer();
};

#endif
