// ServerPointer.h
// Álvaro Castellano Vela - 25/04/2016

#include <string>
#include <iostream>

class ServerPointer {
public:
  ServerPointer(const std::string &, const int &);
  bool point(const std::string &, const std::string &);
  bool unpoint(const std::string &);

private:
  bool call(const std::string &, std::string &);

  std::string server;
  int port;
};
