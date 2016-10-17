// AnsibleHandler.cpp
// Álvaro Castellano Vela 18/10/2016

#define BOOST_SP_USE_QUICK_ALLOCATOR
#include "DatabaseHandler.h"

#include <string>
#include <vector>
#include <boost/make_shared.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <DatabaseHandler.h>

extern VPNQueue managerLogQueue;
extern VPNLock memoryLock;

//contructor
bool ansibleHandler( boost::shared_ptr<std::string> token, DatabaseHandler *db )
{
  time_t now;
  tm *ltm;

  std::string filename;

  now = time(0);
  ltm = localtime(&now);

  // setting filename
  memoryLock.getLock();
  filename = std::string("serverName_") + std::to_string(1900 + ltm->tm_year) + std::string("-");
  memoryLock.releaseLock();

  if (ltm->tm_mon < 10)
  {
    memoryLock.getLock();
    filename = filename + std::string("0");
    memoryLock.releaseLock();
  }

  memoryLock.getLock();
  filename = filename + std::to_string(ltm->tm_mon) + std::string("-");
  memoryLock.releaseLock();

  if (ltm->tm_mday < 10)
  {
     memoryLock.getLock();
     filename = filename + string("0");
     memoryLock.releaseLock();
  }

  memoryLock.getLock();
  filename = filename + to_string(ltm->tm_mday) + string("_");
  memoryLock.releaseLock();

  if (ltm->tm_hour < 10)
  {
    memoryLock.getLock();
    filename = filename + string("0");
    memoryLock.releaseLock();
  }
}
