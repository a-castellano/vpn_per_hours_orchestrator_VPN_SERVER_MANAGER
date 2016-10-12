// VPNManagerDeamon.cpp
// Álvaro Castellano Vela - 24/06/2016

#define BOOST_SP_USE_QUICK_ALLOCATOR
#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

// Project libraries
#include <ManagerNode.h>
#include <VPNLock.h>

using namespace std;

// Global variables

VPNLock curlLock; // Delete this ASAP

// Lock to prevent data reaces
VPNLock memoryLock;

// The queue used by processer thread to write its logs
VPNQueue processerLogQueue;

// The queue used by porcesser for sending messages to manager thread
VPNQueue managerQueue;

// The queue user by manager thread to write its logs
VPNQueue managerLogQueue;

// Support Functions

bool legal_int(char *str) {
  while (*str != 0) {
    if (!(isdigit(*str++))) {
      return false;
    }
  }

  return true;
}

// Main function

// There will be only one argument, the port number.

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  if (!legal_int(argv[1])) {
    return 1;
  }
  /*There are three threads
   * Manager:    the manager read will be reading request untile the request
   * "__KILL_YOURSELF__" arrives
   *             Manager also send the request to the processer thread
   * Processer:  This is the thread which handles the requests
   * Logger:     the logger writes what is happeing in a log file
   */

  boost::thread processer;
  boost::thread manager;
  boost::thread logger;

  // The path where logs will be written
  string logFolder = string("log/Manager/");

  std::stringstream ss;
  unsigned int portnumber = atoi(argv[1]);

  processer = boost::thread(processRequests, portnumber);
  manager = boost::thread(requestManager);
  logger = boost::thread( logManager );

  cout << "Port Number: " << portnumber << endl;

  processer.join();
  manager.join();
  logger.join();

  cout << "End" << endl;

  return 0;
}
