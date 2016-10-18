// AnsibleHandler.h
// Álvaro Castellano Vela 18/10/2016

#include <DatabaseHandler.h>
#include <boost/shared_ptr.hpp>
#include <string>

class AnsibleHandler {
public:
  AnsibleHandler(boost::shared_ptr<std::string>, DatabaseHandler *, const std::string &);

  // Nect version of run function will allow to choose what type of VPN has to be configured
  bool run(void);

private:
  boost::shared_ptr<std::string> token;
  DatabaseHandler *db;
  std::string filename;
  std::string serverIP;
};
