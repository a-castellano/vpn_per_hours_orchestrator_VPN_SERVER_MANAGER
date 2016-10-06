// ManagerMode.h
// Álvaro Castellano Vela - 24/06/2016

#ifndef MANAGERNODE_H
#define MANAGERNODE_H
#endif

#include <string>
#include <queue>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class VPNQueue
{
  public:
    void Enqueue( boost::shared_ptr< std::string > );
    boost::shared_ptr< std::string > Dequeue( );
    bool empty();

  private:
    std::queue< boost::shared_ptr< std::string > > r_queue;
    boost::mutex r_mutex;
};




bool processRequests(const unsigned int , const unsigned int);
/* "processRequests" function opens one socket for receiving 
 * server requests from the distributor, when a request is received
 * it is validated and enqueued.
 *
 * If the received request is "__KILL_YOURSELF__" the manager will send the same message
 * to all the requestsManagers wich will commit suicide
*/
void requestManager( const unsigned int );

void logManager( const std::string & );

bool writeLog( boost::shared_ptr< std::string >, boost::shared_ptr< std::string > );
