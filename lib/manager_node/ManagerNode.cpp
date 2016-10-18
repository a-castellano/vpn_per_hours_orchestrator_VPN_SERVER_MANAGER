// ManagerMode.cpp
// Álvaro Castellano Vela - 24/06/2016

#define BOOST_SP_USE_QUICK_ALLOCATOR
#include "ManagerNode.h"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

//#include <boost/asio.hpp>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h> //usleep

// VPN PROJECT LIBRARIES
#include <DatabaseHandler.h>
#include <ServerFactory.h>
#include <ServerRequest.h>
#include <VPNLock.h>
#include <VPNQueue.h>
#include <AnsibleHandler.h>

extern VPNQueue processerLogQueue;
extern VPNQueue managerQueue;
extern VPNQueue managerLogQueue;
extern VPNLock memoryLock;
extern VPNLock curlLock;
// Functions

/*void VPNQueue::Enqueue(const boost::shared_ptr<std::string> &request) {
  r_mutex.lock();
  r_queue.push(request);
  r_mutex.unlock();
}

boost::shared_ptr<std::string> VPNQueue::Dequeue() {
  boost::shared_ptr<std::string> data;
  r_mutex.lock();
  while (r_queue.empty()) {
    r_mutex.unlock();
    usleep(2000);
    r_mutex.lock();
  }
  data = r_queue.front();
  r_queue.pop();
  r_mutex.unlock();
  return data;
}

bool VPNQueue::empty() {
  bool is_empty;
  r_mutex.lock();
  is_empty = r_queue.empty();
  r_mutex.unlock();
  return is_empty;
}
*/

/*
 *
 * processRequests
 *
 */

bool processRequests(const unsigned int port) {

  int sockfd, newsockfd;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n_read;

  // Log variables

  boost::shared_ptr<std::string> log;
  std::string kill_yourself("__KILL_YOURSELF__");

  // Processer variables
  boost::shared_ptr<std::string> request;
  std::string message("ACK");
  std::string received("Request received: ");

  // The function begins

  memoryLock.getLock();
  log = boost::make_shared<std::string>("Proccesser started!");
  processerLogQueue.Enqueue(log);
  memoryLock.releaseLock();

  // There is only one processer and one manager so maybe there is no need to
  // enqueue the logfile
  memoryLock.getLock();
  log.reset();
  memoryLock.releaseLock();

  // Creating socket

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // clear address structure
  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "ERROR on binding" << std::endl;
  }
  listen(sockfd, 5000);

  clilen = sizeof(cli_addr);

  try {

    for (;;) {
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
      bzero(buffer, 256);
      n_read = read(newsockfd, buffer, 255);
      close(newsockfd);

      memoryLock.getLock();
      request = boost::make_shared<std::string>(buffer);
      memoryLock.releaseLock();

      if (*request == kill_yourself) {
        memoryLock.getLock();
        log = boost::make_shared<std::string>(
            "Sending killing message to the manager.");
        memoryLock.releaseLock();

        processerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        request.reset();
        memoryLock.releaseLock();

        // Sending killing message to the manager.

        memoryLock.getLock();
        request = boost::make_shared<std::string>(kill_yourself);
        memoryLock.releaseLock();

        managerQueue.Enqueue(request);

        memoryLock.getLock();
        request.reset();
        memoryLock.releaseLock();

        // Sending killing message to logger

        memoryLock.getLock();
        log = boost::make_shared<std::string>(kill_yourself);
        memoryLock.releaseLock();

        processerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();
        break;
      }

      else // request is not KILL_YOURSELF
      {
        memoryLock.getLock();
        log = boost::make_shared<std::string>(*request);
        memoryLock.releaseLock();

        processerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();

        managerQueue.Enqueue(request);
        request.reset();
      }

      memoryLock.getLock();
      log = boost::make_shared<std::string>("Request enqueued.");
      memoryLock.releaseLock();

      processerLogQueue.Enqueue(log);
      log.reset();
    } // for

  } catch (std::exception &e) {

    memoryLock.getLock();
    log = boost::make_shared<std::string>(e.what());
    memoryLock.releaseLock();

    processerLogQueue.Enqueue(log);
    memoryLock.getLock();
    log.reset();
    memoryLock.releaseLock();
    return false;
  }

  /*
   * Processer has received kill message
   * It has already send the kill message to manager
   *
   */

  memoryLock.getLock();
  log = boost::make_shared<std::string>("Kill message received... R.I.P.");
  memoryLock.releaseLock();

  processerLogQueue.Enqueue(log);
  memoryLock.getLock();
  log.reset();
  memoryLock.releaseLock();

  close(sockfd);

  kill_yourself.clear();

  return true;
}

/*
 *
 *  requestManager
 *
 */

void requestManager() {

  boost::shared_ptr<std::string> request;

  // This wont be here in next version
  std::string address("paula.es.una.ninja");
  std::string user("vpn");
  std::string password("XLRc3H1y4Db0G4qR630Qk2xPF3D88P");
  std::string database("vpn");

  DatabaseHandler *db;
  DatabaseHandler *db_zones;

  unsigned int zone;
  std::vector<std::string> providers;
  std::string selectedProvider;
  unsigned int providerRandomId;

  std::string kill_yourself("__KILL_YOURSELF__");
  std::string processed("Request totally processed.");
  std::string received("Request reveived -> ");
  std::string correct("The request is correct");
  std::string incorrect("The request is not correct");
  std::string errordb("Error on database connection");

  boost::shared_ptr<ServerRequest> serverRequest;
  Server *server;

  // Database variables
  std::string string_type("string");
  std::string integer_type("integer");

  std::string name_field("name");
  std::string machine_id_field("machine_id");
  std::string ip_field("ip");
  std::string true_zone_field("true_zone");
  std::string provider_field("provider");
  std::string status_field("status");

  // ServerRequest variables
  boost::shared_ptr<std::string> command;
  boost::shared_ptr<std::string> token;

  // Server variables
  boost::shared_ptr<std::string> severName;
  boost::shared_ptr<std::string> serverIP;

  // Log variables
  boost::shared_ptr<std::string> log;
  std::string noPointerStarted = std::string("Manager started.");
  std::string providersTitle = std::string("Providers:");
  std::string selected_provider_string = std::string("Selected provider");
  std::string creating_server_string = std::string("Creating server.");
  std::string setting_up_string = std::string("Setting up server.");
  std::string server_created_string = std::string("Server created.");

  // Ansible
  AnsibleHandler *ansible;

  memoryLock.getLock();
  log = boost::make_shared<std::string>(noPointerStarted);
  memoryLock.releaseLock();
  managerLogQueue.Enqueue(log);
  memoryLock.getLock();
  log.reset();
  memoryLock.releaseLock();

  for (;;) // I will be here forever
  {
    request.reset();
    request = managerQueue.Dequeue();

    if (*request == kill_yourself) {
      memoryLock.getLock();
      log = boost::make_shared<std::string>("Kill message received... R.I.P.");
      memoryLock.releaseLock();

      managerLogQueue.Enqueue(log);

      memoryLock.getLock();
      log.reset();
      memoryLock.releaseLock();

      // Sending killing message to logger

      memoryLock.getLock();
      log = boost::make_shared<std::string>(*request);
      memoryLock.releaseLock();

      managerLogQueue.Enqueue(log);

      memoryLock.getLock();
      log.reset();
      memoryLock.releaseLock();

      break;
    }

    memoryLock.getLock();
    log = boost::make_shared<std::string>(*request);
    memoryLock.releaseLock();

    managerLogQueue.Enqueue(log);

    memoryLock.getLock();
    log.reset();
    memoryLock.releaseLock();

    // Process the request

    serverRequest = boost::make_shared<ServerRequest>(*request, 1);

    if (serverRequest->isCorrect()) {
      memoryLock.getLock();
      log = boost::make_shared<std::string>(correct);
      memoryLock.releaseLock();

      managerLogQueue.Enqueue(log);

      memoryLock.getLock();
      log.reset();
      memoryLock.releaseLock();

      memoryLock.getLock();
      command = boost::make_shared<std::string>(serverRequest->getCommand());
      token = boost::make_shared<std::string>(serverRequest->getToken());
      log = boost::make_shared<std::string>(*command);
      memoryLock.releaseLock();

      managerLogQueue.Enqueue(log);

      memoryLock.getLock();
      log.reset();
      memoryLock.releaseLock();

      memoryLock.getLock();
      db = new DatabaseHandler(address, 3306, user, password, database);
      memoryLock.releaseLock();

      if (!db->dataIsWellFormed()) {
        memoryLock.getLock();
        log = boost::make_shared<std::string>(errordb);
        memoryLock.releaseLock();

        managerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();

        memoryLock.getLock();
        delete (db);
        memoryLock.releaseLock();
      }

      else {

        // Continue handling the request

        memoryLock.getLock();
        zone = db->getServerZoneFromToken(token);
        severName = db->setServerName(token, zone);
        memoryLock.releaseLock();

        memoryLock.getLock();
        log = boost::make_shared<std::string>(*severName);
        memoryLock.releaseLock();

        managerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();

        memoryLock.getLock();
        db->updateDBField(token, name_field, string_type, severName);
        memoryLock.releaseLock();

        memoryLock.getLock();
        delete (db);
        memoryLock.releaseLock();

        memoryLock.getLock();
        db_zones = new DatabaseHandler(address, 3306, user, password,
                                       std::string("vpn_zones"));
        memoryLock.releaseLock();

        memoryLock.getLock();
        providers = db_zones->getProvidersFromZone(zone);
        memoryLock.releaseLock();

        memoryLock.getLock();
        log = boost::make_shared<std::string>(providersTitle);
        memoryLock.releaseLock();

        managerLogQueue.Enqueue(log);

        for (std::string provider : providers) {
          memoryLock.getLock();
          log = boost::make_shared<std::string>(provider);
          memoryLock.releaseLock();

          managerLogQueue.Enqueue(log);
        };

        memoryLock.getLock();
        delete (db_zones);
        memoryLock.releaseLock();

        if (providers.size() == 1) {
          selectedProvider = providers[0];
        } else {
          srand(time(NULL));
          providerRandomId = rand() % (providers.size());
          selectedProvider = providers[providerRandomId];
        }

        memoryLock.getLock();
        log = boost::make_shared<std::string>(selected_provider_string);
        memoryLock.releaseLock();

        managerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();

        memoryLock.getLock();
        log = boost::make_shared<std::string>(selectedProvider);
        memoryLock.releaseLock();

        managerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();

        memoryLock.getLock();
        log = boost::make_shared<std::string>(creating_server_string);
        memoryLock.releaseLock();

        managerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();

        memoryLock.getLock();
        server = CreateServer(selectedProvider, token);
        memoryLock.releaseLock();

        if (!server) {
          std::cerr << "Server object not created. Aborting." << std::endl;
          // Notify error
        } else {
          memoryLock.getLock();
          server->setZone(zone);
          server->setServerName(*severName);
          memoryLock.releaseLock();

          memoryLock.getLock();
          server->create();
          memoryLock.releaseLock();

          memoryLock.getLock();
          log = boost::make_shared<std::string>(server_created_string);
          memoryLock.releaseLock();

          managerLogQueue.Enqueue(log);

          memoryLock.getLock();
          log.reset();
          memoryLock.releaseLock();

          //Updating db fields
          memoryLock.getLock();
          db = new DatabaseHandler(address, 3306, user, password, database);
          memoryLock.releaseLock();

          memoryLock.getLock();
          log = boost::make_shared<std::string>(machine_id_field + std::string(": ") + std::string(server->getMachineID()));
          db->updateDBField(token, machine_id_field, integer_type, server->getMachineID());
          memoryLock.releaseLock();

          managerLogQueue.Enqueue(log);

          memoryLock.getLock();
          log.reset();
          memoryLock.releaseLock();

          memoryLock.getLock();
          log = boost::make_shared<std::string>(ip_field + std::string(": ") + std::string(server->getServerIP()));
          db->updateDBField(token, ip_field, string_type, server->getServerIP());
          serverIP = boost::make_shared<std::string>(server->getServerIP());
          memoryLock.releaseLock();

          managerLogQueue.Enqueue(log);

          memoryLock.getLock();
          log.reset();
          memoryLock.releaseLock();

          memoryLock.getLock();
          log = boost::make_shared<std::string>(true_zone_field + std::string(": ") + std::string(server->getTrueZone()));
          db->updateDBField(token, true_zone_field, string_type, server->getTrueZone());
          memoryLock.releaseLock();

          managerLogQueue.Enqueue(log);

          memoryLock.getLock();
          log.reset();
          memoryLock.releaseLock();

          memoryLock.getLock();
          log = boost::make_shared<std::string>(provider_field + std::string(": ") + std::string(selectedProvider));
          db->updateDBField(token, provider_field, string_type, selectedProvider);
          memoryLock.releaseLock();

          managerLogQueue.Enqueue(log);

          memoryLock.getLock();
          log.reset();
          memoryLock.releaseLock();

          memoryLock.getLock();
          log = boost::make_shared<std::string>(status_field + std::string(": ") + std::string(setting_up_string));
          db->updateDBField(token, status_field, string_type, setting_up_string);
          memoryLock.releaseLock();

          managerLogQueue.Enqueue(log);

          memoryLock.getLock();
          log.reset();
          memoryLock.releaseLock();

          // Ansible uses memoryLock too
          ansible = new AnsibleHandler(token, db, *serverIP);

          // Ansible run is using memoryLock
          ansible->run();

          memoryLock.getLock();
          delete (ansible);
          memoryLock.releaseLock();

          memoryLock.getLock();
          delete (db);
          memoryLock.releaseLock();

        }
      }

    } else { // Server Request is not correct

      memoryLock.getLock();
      log = boost::make_shared<std::string>(incorrect);
      memoryLock.releaseLock();

      managerLogQueue.Enqueue(log);

      memoryLock.getLock();
      log.reset();
      memoryLock.releaseLock();
    }

    memoryLock.getLock();
    serverRequest.reset();
    memoryLock.releaseLock();

    usleep((rand() % 10 + 1) * 10000);

    memoryLock.getLock();
    log = boost::make_shared<std::string>(processed);
    memoryLock.releaseLock();

    managerLogQueue.Enqueue(log);
    memoryLock.getLock();
    log.reset();
    request.reset();
    memoryLock.releaseLock();

  } // for
}

void logManager() {

  boost::shared_ptr<std::string> log;
  std::string logFileManager("log/Manager.log");
  std::string logFileProcesser("log/Processer.log");
  std::string kill_yourself("__KILL_YOURSELF__");

  std::ofstream outfileManager(logFileManager);
  std::ofstream outfileProcesser(logFileProcesser);

  if (!outfileManager.is_open()) {
    std::cerr << "Couldn't open '" << logFileManager << "'." << std::endl;
    return;
  }

  if (!outfileProcesser.is_open()) {
    std::cerr << "Couldn't open '" << logFileProcesser << "'." << std::endl;
  }

  unsigned int killed_queues = 0;

  while (killed_queues != 2) // There are only 2 queues
  {
    if (!processerLogQueue.empty()) {
      memoryLock.getLock();
      log = processerLogQueue.Dequeue();
      memoryLock.releaseLock();
      if (*log != kill_yourself) {

        writeLog(outfileProcesser, log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();
      } else {
        killed_queues++;
      }
    }
    if (!managerLogQueue.empty()) {
      memoryLock.getLock();
      log = managerLogQueue.Dequeue();
      memoryLock.releaseLock();
      if (*log != kill_yourself) {

        writeLog(outfileManager, log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();
      } else {
        killed_queues++;
      }
    }
    usleep(1000);
  }
}

bool writeLog(std::ofstream &outfile, boost::shared_ptr<std::string> data) {

  time_t t;
  struct tm *now;

  memoryLock.getLock();
  t = time_t(std::time(0));
  now = std::localtime(&t);
  memoryLock.releaseLock();

  outfile << "[";
  outfile << (now->tm_year + 1900) << '/';
  if (now->tm_mon < 9)

    outfile << "0";

  outfile << (now->tm_mon + 1) << '/';
  if (now->tm_mday < 10)
    outfile << "0";
  outfile << now->tm_mday << '-';
  if (now->tm_hour < 10)
    outfile << "0";
  outfile << now->tm_hour << ':';
  if (now->tm_min < 10)
    outfile << "0";
  outfile << now->tm_min << ':';
  if (now->tm_sec < 10)
    outfile << "0";
  outfile << now->tm_sec;

  outfile << "] - ";
  outfile << *data;
  outfile << "\n";

  return true;
}
