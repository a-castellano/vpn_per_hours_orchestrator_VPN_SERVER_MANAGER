// VPNQueue.cpp
// Álvaro Castellano Vela 18/10/2016

#include "VPNQueue.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <queue>

void VPNQueue::Enqueue(const boost::shared_ptr<std::string> &request) {
  r_mutex.lock();
  r_queue.push(request);
  r_mutex.unlock();
}

boost::shared_ptr<std::string> VPNQueue::Dequeue() {
  boost::shared_ptr<std::string> data;
  r_mutex.lock();
  while (r_queue.empty()) {
    r_mutex.unlock();
    usleep(2000);
    r_mutex.lock();
  }
  data = r_queue.front();
  r_queue.pop();
  r_mutex.unlock();
  return data;
}

bool VPNQueue::empty() {
  bool is_empty;
  r_mutex.lock();
  is_empty = r_queue.empty();
  r_mutex.unlock();
  return is_empty;
}
