// VPNManagerDeamon.cpp
// Álvaro Castellano Vela - 24/06/2016

#define BOOST_SP_USE_QUICK_ALLOCATOR
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

VPNQueue requestsQueue;
vector<VPNQueue*> logQueues;
VPNLock curlLock;
VPNLock memoryLock;
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

int main( int argc, char *argv[] ) // port number and numthreads 
{

  if ( argc != 3)
  {
    return 1;
  }

  if ( ! legal_int( argv[1] ) || ! legal_int( argv[2] ) )
  {
    return 1;
  }

  boost::thread manager;
  boost::thread logger;

  std::stringstream ss;
  unsigned int portnumber = atoi( argv[1] );
  unsigned int numthreads = atoi( argv[2] );

  //VPNQueue *requestsQueue = new VPNQueue();

  VPNQueue *logQueue;// = new VPNQueue();
  //vector<VPNQueue *> logQueues;
  //logQueues.resize(numthreads+1);

  boost::thread_group threads;

  string logFolder= string("log/Manager_")+to_string(portnumber)+string("/");

  boost::shared_ptr< std::string > killMsg;

  for( unsigned int i = 0; i < numthreads ; i++ )
  {
    logQueue = new VPNQueue();
    threads.add_thread( new boost::thread( requestManager, i ) );
    logQueues.push_back( logQueue );
  }

  logQueue = new VPNQueue();
  manager = boost::thread( processRequests, portnumber, numthreads );
  logQueues.push_back( logQueue );

  logger = boost::thread( logManager, logFolder);

  cout << "Port Number: " << portnumber << endl;
  cout << "Number of threads: " << numthreads << endl;
  manager.join();
  threads.join_all();

  killMsg = boost::shared_ptr< std::string >( new std::string( "__KILL_YOURSELF__" ) );
  logQueues[numthreads]->Enqueue(killMsg);
  killMsg.reset();

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
