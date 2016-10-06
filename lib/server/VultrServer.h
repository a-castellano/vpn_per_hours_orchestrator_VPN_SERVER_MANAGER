// VultrServer.h
// Álvaro Castellano Vela 11/03/2016

#ifndef VULTRSERVER_H
#define VULTRSERVER_H

#include "Server.h"
#include <string>

class VultrServer : public Server {
public:
  VultrServer(const std::string &);

  bool create();
  bool destroy();
  bool powerOn();
  bool powerOff();

  const std::string serverType();

  ~VultrServer();

  static Server * Create(const std::string &token) { return new
	  VultrServer(token); }

private:
  const std::string zoneName();
  bool curlGET(const std::string &, std::stringstream &);
  bool curlPOST(const std::string &, const std::string &, std::stringstream &);
};

#endif
