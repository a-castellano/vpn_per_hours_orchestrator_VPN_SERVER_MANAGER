// Álvaro Castellano Vela
// 29/04/2016

#include <iostream>
#include <string>
#include <ServerPointer.h>

using namespace std;

int main() {

  string subdomain("windmaker.net");
  string ip("37.187.126.42");

  ServerPointer *pointer =
      new ServerPointer(string("pointer.windmaker.net"), 8745);

  for (int i = 1; i < 100; i++) {

    if (pointer->point(string("test") + to_string(i) + string(".") + subdomain,
                       ip)) {
      cout << "POINTING SUCCESS" << endl;
    } else {
      cout << "POINTING FAILED" << endl;
    }
  }

  for (int i = 1; i < 100; i++) {
    if (pointer->unpoint(string("test") + to_string(i) + string(".") +
                         subdomain)) {
      cout << "UNPOINTING SUCCESS" << endl;
    } else {
      cout << "UNPOINTING FAILED" << endl;
    }
  }
  delete pointer;

  return 0;
}
