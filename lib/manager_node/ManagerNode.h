// ManagerMode.h
// Álvaro Castellano Vela - 24/06/2016

#ifndef MANAGERNODE_H
#define MANAGERNODE_H
#endif

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <string>

class VPNQueue {
public:
  void Enqueue(const boost::shared_ptr<std::string> &);
  boost::shared_ptr<std::string> Dequeue();
  bool empty();

private:
  std::queue<boost::shared_ptr<std::string>> r_queue;
  boost::mutex r_mutex;
};

bool processRequests(const unsigned int);
/* "processRequests" function opens one socket for receiving
 * server requests from the distributor, when a request is received
 * it is validated and enqueued.
 *
 * If the received request is "__KILL_YOURSELF__" the manager will send the same
 * message
 * to all the requestsManagers wich will commit suicide
*/
void requestManager();

void logManager();

bool writeLog(std::ofstream &, boost::shared_ptr<std::string>);
