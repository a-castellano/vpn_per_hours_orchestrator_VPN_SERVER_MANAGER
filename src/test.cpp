#include <iostream>
#include <ServerRequest.h>
#include <VPNLock.h>

using namespace std;

VPNLock memoryLock;

int main(int argc, char *argv[])
{
	cout << argc << endl;
        if ( argc != 2 )
		return 1;
	
	string request = argv[1];

        ServerRequest test( request );
	cout << "Hola" << endl;
        if ( test.isCorrect() )
	{
		cout << "Command: " << test.getCommand() << endl;
                cout << "Token: " << test.getToken() << endl;
	}
	else
	{
		cout << "REQUEST MAL CONSTRUIDA" << endl;
	}
	
}
