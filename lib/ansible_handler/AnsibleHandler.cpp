// AnsibleHandler.cpp
// Álvaro Castellano Vela 18/10/2016

#define BOOST_SP_USE_QUICK_ALLOCATOR
#include "AnsibleHandler.h"
#include "ansible_files.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include <boost/make_shared.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <DatabaseHandler.h>
#include <VPNLock.h>
#include <VPNQueue.h>

extern VPNQueue managerLogQueue;
extern VPNLock memoryLock;

// contructor
AnsibleHandler::AnsibleHandler(boost::shared_ptr<std::string> token,
                               DatabaseHandler *db, const std::string &serverIP)
    : token(token), db(db), serverIP(serverIP) {
  time_t now;
  tm *ltm;

  now = time(0);
  ltm = localtime(&now);

  // setting filename
  memoryLock.getLock();
  this->filename = std::string("serverName_") +
                   std::to_string(1900 + ltm->tm_year) + std::string("-");
  memoryLock.releaseLock();

  if (ltm->tm_mon < 10) {
    memoryLock.getLock();
    this->filename = this->filename + std::string("0");
    memoryLock.releaseLock();
  }

  memoryLock.getLock();
  this->filename =
      this->filename + std::to_string(ltm->tm_mon) + std::string("-");
  memoryLock.releaseLock();

  if (ltm->tm_mday < 10) {
    memoryLock.getLock();
    this->filename = this->filename + std::string("0");
    memoryLock.releaseLock();
  }
  
  memoryLock.getLock();
  this->filename =
      this->filename + std::to_string(ltm->tm_mday) + std::string("_");
  memoryLock.releaseLock();

  if (ltm->tm_hour < 10) {
    memoryLock.getLock();
    this->filename = this->filename + std::string("0");
    memoryLock.releaseLock();
  }

  memoryLock.getLock();
  this->filename =
      this->filename + std::to_string(ltm->tm_hour) + std::string(":");
  memoryLock.releaseLock();
  
  if (ltm->tm_min < 10) {
    memoryLock.getLock();
    this->filename = filename + std::string("0");
    memoryLock.releaseLock();
  }

  memoryLock.getLock();
  this->filename =
      this->filename + std::to_string(ltm->tm_min) + std::string(":");
  memoryLock.releaseLock();

  if (ltm->tm_sec < 10) {
    memoryLock.getLock();
    this->filename = this->filename + std::string("0");
    memoryLock.releaseLock();
  }

  memoryLock.getLock();
  this->filename = this->filename + std::to_string(ltm->tm_sec);
  memoryLock.releaseLock();
}

bool AnsibleHandler::run() {

  std::vector<unsigned int> VPNusers;
  std::ofstream chap_secrets;
  std::ofstream inventory;
  std::string VPNusername;
  std::string VPNpassword;

  std::string iprange(IP_RANGE);

  unsigned int ipnumber = IPNUMBER;

  std::string vpn_password_file_string("VPN password file: ");
  std::string user_id_string("ID: ");
  std::string username_string("Username: ");
  std::string password_string("Password: ");
  std::string waiting_server_string("Waiting the server");

  boost::shared_ptr<std::string> log;

  std::string chap_secrets_filename =
      std::string(CHAP_SECRETS_FILENAME) + this->filename;
  std::string inventory_filename =
      std::string(INVENTORIES_FILENAME) + this->filename;

  memoryLock.getLock();
  log = boost::make_shared<std::string>(vpn_password_file_string);
  memoryLock.releaseLock();

  managerLogQueue.Enqueue(log);

  memoryLock.getLock();
  log.reset();
  memoryLock.releaseLock();

  memoryLock.getLock();
  log = boost::make_shared<std::string>(chap_secrets_filename);
  memoryLock.releaseLock();

  managerLogQueue.Enqueue(log);

  memoryLock.getLock();
  log.reset();
  memoryLock.releaseLock();

  memoryLock.getLock();
  VPNusers = this->db->getVPNUsers(*token);
  memoryLock.releaseLock();

  memoryLock.getLock();
  chap_secrets.open(chap_secrets_filename.c_str());
  memoryLock.releaseLock();

  for (unsigned int userID : VPNusers) {
    memoryLock.getLock();
    db->getVPNUserPassword(userID, &VPNusername, &VPNpassword);
    memoryLock.releaseLock();

    memoryLock.getLock();
    log = boost::make_shared<std::string>(user_id_string +
                                          std::to_string(userID));
    memoryLock.releaseLock();

    managerLogQueue.Enqueue(log);

    memoryLock.getLock();
    log.reset();
    memoryLock.releaseLock();

    memoryLock.getLock();
    log = boost::make_shared<std::string>(username_string + VPNusername);
    memoryLock.releaseLock();

    managerLogQueue.Enqueue(log);

    memoryLock.getLock();
    log.reset();
    memoryLock.releaseLock();

    memoryLock.getLock();
    log = boost::make_shared<std::string>(password_string + VPNpassword);
    memoryLock.releaseLock();

    managerLogQueue.Enqueue(log);

    memoryLock.getLock();
    log.reset();
    memoryLock.releaseLock();

    memoryLock.getLock();
    chap_secrets << VPNusername << "\t*\t" << VPNpassword << "\t*\t" << iprange
                 << ipnumber << "\n";
    memoryLock.releaseLock();

    ipnumber++;

  } //  for (unsigned int userID : VPNusers)

  memoryLock.getLock();
  chap_secrets.close();
  memoryLock.releaseLock();

  memoryLock.getLock();
  inventory.open(inventory_filename.c_str());
  memoryLock.releaseLock();

  memoryLock.getLock();
  inventory << "[target]\n" << this->serverIP << "\n";
  memoryLock.releaseLock();

  memoryLock.getLock();
  inventory.close();
  memoryLock.releaseLock();

  memoryLock.getLock();
  log = boost::make_shared<std::string>(waiting_server_string);
  memoryLock.releaseLock();
}
