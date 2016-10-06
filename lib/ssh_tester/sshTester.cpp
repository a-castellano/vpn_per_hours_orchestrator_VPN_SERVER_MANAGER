// sshTester.cpp
// Álvaro Castellano Vela - 08/05/2016

#define LIBSSH_STATIC 1

#include <string>
#include <libssh/libssh.h>
#include <iostream>
#include "sshTester.h"

bool testSSHConection(const std::string &host, const int &port) {
  ssh_session my_ssh_session;
  int rc;
	int verbosity = SSH_LOG_PROTOCOL;

  my_ssh_session = ssh_new();
  if (my_ssh_session == NULL) {
    return false;
  }

  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host.c_str());
	//ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

	rc = ssh_connect(my_ssh_session);
  if (rc != SSH_OK) {
    return false;
  } else {
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
  }
  return true;
}
