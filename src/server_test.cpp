#include <iostream>
#include <string>

#include <ServerFactory.h>
//#include <Server.h>
//#include <DigitalOceanServer.h>
//#include <VultrServer.h>

using namespace std;

int main(void) {
  string token("328482348239h42332h7326723hgh");
	string provider("DigitalOcean");
	string provider2("Vultr");

/*
  Server *server = new DigitalOceanServer();
  Server *vserver = new VultrServer();

	server->setToken(token);
	vserver->setToken(token); 
*/
  cout << "hola" << endl;

	Server *server = CreateServer(provider,token);
	Server *vserver = CreateServer(provider2,token);   
//	server->setToken(token);

  cout << server->getToken() << endl;
  cout << server->serverType() << endl;

	cout << "otro" << endl;

  cout << vserver->getToken() << endl;
  cout << vserver->serverType() << endl;
}
