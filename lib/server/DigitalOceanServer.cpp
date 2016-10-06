// DigitalOceanServer.cpp
// Álvaro Castellano Vela 12/03/2016

#include "DigitalOceanServer.h"

DigitalOceanServer::DigitalOceanServer(const std::string &token)
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
