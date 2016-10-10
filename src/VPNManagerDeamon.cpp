// VPNManagerDeamon.cpp
// Álvaro Castellano Vela - 24/06/2016

//#define BOOST_SP_USE_QUICK_ALLOCATOR
#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

// Project libraries
#include <VPNLock.h>
#include <ManagerNode.h>

using namespace std;

// Global variables

VPNLock curlLock;
VPNLock memoryLock;

// The queue used by processer thread to write its logs
VPNQueue processerLogQueue;

// The queue used by porcesser for sending messages to manager thread
VPNQueue managerQueue;

// The queue user by manager thread to write its logs
VPNQueue managerLogQueue;


// Support Functions

bool legal_int(char *str)
{
  while (*str != 0)
  {
    if( ! (isdigit(*str++)) )
    {
      return false;
    }
  }

  return true;
}

// Main function

// There will be only one argument, the port number.
// In the near future there will not be arguments, the port will be the same always

int main( int argc, char *argv[] )
{
  if ( argc != 2)
  {
    return 1;
  }
  if ( ! legal_int( argv[1] ) ) // The port number is a valid int
  {
    return 1;
  }
  /*There are two threads
   * Manager: the manager read will be reading request untile the request "__KILL_YOURSELF__" arrives
   * Logger: the logger writes what is happeing in a log file
   */

  boost::thread manager;
  boost::thread request_manager;
  boost::thread logger;

  // The path where logs will be written
  string logFolder= string("log/Manager/"); 

  std::stringstream ss;
  unsigned int portnumber = atoi( argv[1] ); //TODO -> DELETE PORTNUMBER

  //boost::shared_ptr< std::string > killMsg;

  //for( unsigned int i = 0; i < numthreads ; i++ )
  //{
  //  logQueue = new VPNQueue();
  //  threads.add_thread( new boost::thread( requestManager, i ) );
  //  logQueues.push_back( logQueue );
  //}

  manager = boost::thread( processRequests, portnumber );
  request_manager = boost::thread( requestManager );
  logger = boost::thread( logManager );

  cout << "Port Number: " << portnumber << endl;
  manager.join();
  request_manager.join();

//  killMsg = boost::shared_ptr< std::string >( new std::string( "__KILL_YOURSELF__" ) );

  logger.join();

  //for( unsigned int i = 0; i < logQueues.size() ; i++ )
  //{
  //  logQueue = logQueues[i];
  //  free( logQueue );
  //}

  //free(curlLock);
  //free(requestLock);
  //free(memoryLock);

  cout << "End" << endl;

  return 0;
}
