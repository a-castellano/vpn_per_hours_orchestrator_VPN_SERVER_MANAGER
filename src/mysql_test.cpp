#include <iostream>
#include <vector>
#include <string>

#include <DatabaseHandler.h>

using namespace std;

int main(int argc, char *argv[]) {
  cout << argc << endl;
  if (argc != 6)
    return 1;

  string address(argv[1]);
  string user(argv[2]);

  string password(argv[3]);
  string database(argv[4]);

  string token(argv[5]);

  unsigned int zone;

  cout << user.length() << endl;
  DatabaseHandler db(address, 3306, user, password, database);

  if (db.dataIsWellFormed()) {
    cout << "BIEN CONSTRUIDO" << endl;
  }
  if (db.successConected()) {
    cout << "CONECTADO" << endl;
  } else {
    cout << "NO CONECTADO!" << endl;
  }

  zone = db.getServerZoneFromToken(token);

  if (db.hasError()) {
    cout << "TENEMOS UN ERROR" << endl;
  }

  cout << "ZONA -> " << zone << endl;

  vector<string> providers;

  zone = 1;

  DatabaseHandler db2(address, 3306, user, password, string("vpn_zones"));
  providers = db2.getProvidersFromZone(zone);
  cout << "Poviders: " << providers.size() << endl;
  for (string i : providers) {
    cout << i << endl;
  }

  return EXIT_SUCCESS;
}
