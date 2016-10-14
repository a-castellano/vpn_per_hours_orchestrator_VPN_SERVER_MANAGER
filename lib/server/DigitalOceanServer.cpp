// DigitalOceanServer.cpp
// Álvaro Castellano Vela 12/03/2016

#include "DigitalOceanServer.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

DigitalOceanServer::DigitalOceanServer(boost::shared_ptr<std::string> token)
    : Server(token) {}

const std::string DigitalOceanServer::serverType() {
  return std::string("DigitalOcean");
}

DigitalOceanServer::~DigitalOceanServer(){}

    bool DigitalOceanServer::create() {
  return true;
}

bool DigitalOceanServer::destroy() { return true; }

bool DigitalOceanServer::powerOn() { return true; }

bool DigitalOceanServer::powerOff() { return true; }
