// Logger.cpp
// Alvaro Castellano Vela - 02/04/2016

#include "Log.h"
#include "Logger.h"

using namespace logging::trivial;

bool writeLog(const std::string & file, const std::string &msg)
{
	logging::add_common_attributes();
  std::cout << file << std::endl;
	dhlogging::Logger::getInstance(file)->logInfo(msg);
	return true;
}
