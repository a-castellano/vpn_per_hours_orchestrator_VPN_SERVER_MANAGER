// serverRequest.h
// Álvaro Castellano Vela 23/02/2016
#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H
#endif

#ifndef STRING
#include <string>
#endif

#ifndef VECTOR
#include <vector>
#endif

#ifndef BOOST_ALGORITHM_STRING
#include <boost/algorithm/string.hpp>
#endif

class ServerRequest {
public:
  explicit ServerRequest(const std::string &);
  ServerRequest(const std::string &, const int &);
  const bool isCorrect(void);
  const std::string getCommand(void);
  const std::string getToken(void);

private:
  std::vector<std::string> processedRequest;
  bool correctRequest;
};
