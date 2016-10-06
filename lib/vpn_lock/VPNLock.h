// VPNLock.h
// Álvaro Castellano Vela - 26/07/2016

#ifndef VPNLOCK_H
#define VPNLOCK_H
#endif

#include <boost/thread.hpp>

class VPNLock
{
  public:
    void getLock();
    void releaseLock();

  private:
    boost::shared_mutex c_mutex;
};


