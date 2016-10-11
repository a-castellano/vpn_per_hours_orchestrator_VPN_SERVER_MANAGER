// VPNLock.cpp
// Álvaro Castellano Vela

#define BOOST_SP_USE_QUICK_ALLOCATOR
#include "VPNLock.h"

void VPNLock::getLock() { c_mutex.lock(); }

void VPNLock::releaseLock() { c_mutex.unlock(); }


