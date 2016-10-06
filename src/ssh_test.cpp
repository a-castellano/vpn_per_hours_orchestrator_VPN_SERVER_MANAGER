#include <string>
#include <iostream>

#include <sshTester.h>

using namespace std;

int main(int argc, const char *argv[])
{
	
	string host;
	
	if ( argc != 2 )
	{
		return 0;
	}
	else
	{
		host = string(argv[1]);
		testSSHConection(host,22);
		return 1;
	}
}
