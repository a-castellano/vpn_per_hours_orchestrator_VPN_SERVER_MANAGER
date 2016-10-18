// VPNQueue.h
// Álvaro Castellano Vela 18/10/2016

//#ifndef VPNQUEUE
#define VPNQUEUE

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <queue>

class VPNQueue {
public:
  void Enqueue(const boost::shared_ptr<std::string> &);
  boost::shared_ptr<std::string> Dequeue();
  bool empty();

private:
  std::queue<boost::shared_ptr<std::string>> r_queue;
  boost::mutex r_mutex;
};

//#endif
