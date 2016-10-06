#include <iostream>
#include <string>

//#include <Log.h>
#include <Logger.h>

using namespace std; 

int main ()
{
	string file("/var/log/vpnporhours.log");
	string msg("YOLO");
	string msg2("YOOOOLO 2");
	writeLog(file,msg);	
	writeLog(file,msg2);

	return 0;
}

/*
#include <Log.h>

using namespace std;


int main(int argc, char *argv[])
{
	logging::add_common_attributes();  

	    using namespace logging::trivial;
	dhlogging::Logger::getInstance()->logInfo("himom");	

	return 0;
}
*/
