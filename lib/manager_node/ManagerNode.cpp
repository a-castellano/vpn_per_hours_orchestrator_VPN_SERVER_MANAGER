// ManagerMode.cpp
// Álvaro Castellano Vela - 24/06/2016

//#define BOOST_SP_USE_QUICK_ALLOCATOR
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

extern VPNQueue processerLogQueue;
extern VPNQueue managerQueue;
extern VPNQueue managerLogQueue;
extern VPNLock memoryLock;
extern VPNLock curlLock;
// Functions

void VPNQueue::Enqueue(boost::shared_ptr<std::string> request) {
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

/*
 *
 * processRequests
 *
 */

bool processRequests(const unsigned int port ) {

  int sockfd, newsockfd;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;//WTF is n?

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

  //There is only one processer and one manager so maybe there is no need to enqueue the logfile
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

  // newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

  // boost::asio::io_service io_service;
  // tcp::endpoint endpoint(tcp::v4(), port);
  // tcp::acceptor acceptor(io_service, endpoint);

  // tcp::socket socket(io_service);
  // tcp::iostream stream;

  // boost::system::error_code ignored_error;

  try {

    for (;;) {
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
      bzero(buffer, 256);
      n = read(newsockfd, buffer, 255);
      close(newsockfd);

      memoryLock.getLock();
      request = boost::make_shared<std::string>(buffer);
      memoryLock.releaseLock();

      memoryLock.getLock();
      log = boost::make_shared<std::string>(/*received +*/ *request);
      memoryLock.releaseLock();

      processerLogQueue.Enqueue(log);
      memoryLock.getLock();
      log.reset();
      memoryLock.releaseLock();

      if (*request == kill_yourself) {
        memoryLock.getLock();
        log =
            boost::make_shared<std::string>("Sending killing message to the manager.");
        memoryLock.releaseLock();

        processerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();

        request.reset();
        memoryLock.releaseLock();

        //for (unsigned int t = 0; t < numthreads; t++) {
          memoryLock.getLock();
          request = boost::make_shared<std::string>(kill_yourself);
          memoryLock.releaseLock();

          managerQueue.Enqueue(request);
          memoryLock.getLock();
          request.reset();
          memoryLock.releaseLock();
        //}

        break;
      }

      else // request is not KILL_YOURSELF
      {
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

  memoryLock.getLock();
  log = boost::make_shared<std::string>("Proccesser finished.");
  memoryLock.releaseLock();

  processerLogQueue.Enqueue(log);
  memoryLock.getLock();
  log.reset();
  memoryLock.releaseLock();

  // The only time we enqueue one unique item

  memoryLock.getLock();
  memoryLock.releaseLock();

  memoryLock.getLock();
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

  boost::shared_ptr<std::string> command;
  boost::shared_ptr<std::string> token;


  //This wont be here in next version
  std::string address("paula.es.una.ninja");
  std::string user("vpn");
  std::string password("XLRc3H1y4Db0G4qR630Qk2xPF3D88P");
  std::string database("vpn");

  DatabaseHandler * db;
  DatabaseHandler * db_zones;

  unsigned int zone;
  std::vector<std::string> providers;
  std::string selectedProvider;
  unsigned int providerRandomId;

  boost::shared_ptr<std::string> severName;

  std::string kill_yourself("__KILL_YOURSELF__");
  std::string processed("Request totally processed.");
  std::string received("Request reveived -> ");
  std::string correct("The request is correct");
  std::string incorrect("The request is not correct");
  std::string errordb("Error on database connection");

  std::string noPointerStarted = std::string("Manager started.");

  boost::shared_ptr<ServerRequest> serverRequest;
  boost::shared_ptr<Server> server;
  // Server *server;

  boost::shared_ptr<std::string> log;

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

    memoryLock.getLock();
    log = boost::make_shared<std::string>(/*received +*/ *request);
    memoryLock.releaseLock();

    managerLogQueue.Enqueue(log);
    memoryLock.getLock();
    log.reset();
    memoryLock.releaseLock();

    if (*request == kill_yourself) {
      memoryLock.getLock();
      log = boost::make_shared<std::string>("Kill message received... R.I.P.");
      memoryLock.releaseLock();

      managerLogQueue.Enqueue(log);

      memoryLock.getLock();
      log.reset();
      memoryLock.releaseLock();

      break;
    }

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
      db = new DatabaseHandler(address, 3306, user, password,
                                               database);
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
        delete(db);
        memoryLock.releaseLock();
      }

      else {
        memoryLock.getLock();
        zone = db->getServerZoneFromToken(*token);
        severName = boost::make_shared<std::string>(db->setServerName(*token, zone));
        memoryLock.releaseLock();

        memoryLock.getLock();
        log = boost::make_shared<std::string>(*severName);
        memoryLock.releaseLock();

        managerLogQueue.Enqueue(log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();

        memoryLock.getLock();
        delete(db);
        memoryLock.releaseLock();
      }

    } else { //Server Request is not correct

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

    /*
        ServerRequest *serverRequest = new ServerRequest( request );

        if ( serverRequest->isCorrect() ) {

          command = serverRequest->getCommand();
          token = serverRequest->getToken();

          log = std::string("Commad: ") + command;
          logQueue->Enqueue( logFile + std::string("__-*-__") + log );

          log = std::string("Token: ") + token;
          logQueue->Enqueue( logFile + std::string("__-*-__") + log );

          free( serverRequest );

          db = new DatabaseHandler(address, 3306, user, password, database);

          if (!db->dataIsWellFormed()) {
            log = std::string("Error: Database data is incorrect.");
            logQueue->Enqueue( logFile + std::string("__-*-__") + log );
            //Send some alert
          }
          else {
            zone = db->getServerZoneFromToken(token);
            if (db->hasError()) {
              log = std::string("Error: getServerZoneFromToken.");
              logQueue->Enqueue( logFile + std::string("__-*-__") + log );
              free(db);
            }
            else {
              log = std::string("Zone: ") + std::to_string(zone);
              logQueue->Enqueue( logFile + std::string("__-*-__") + log );
              severName = db->setServerName(token, zone);
              log = std::string("Server Name: ") + severName;
              logQueue->Enqueue( logFile + std::string("__-*-__") + log );
              //free(db);

              //db = new DatabaseHandler(address, 3306, user, password,
       database);
              db->updateDBField(token, std::string("name"),
       std::string("string"), severName);
              db->updateDBField(token, std::string("name"),
       std::string("string"), severName);
              free(db);

              db_zones = new DatabaseHandler(address, 3306, user, password,
       std::string("vpn_zones"));
              providers = db_zones->getProvidersFromZone(zone);

              log = std::string("Providers: ");
              logQueue->Enqueue( logFile + std::string("__-*-__") + log );

              for (std::string provider : providers) {
                logQueue->Enqueue( logFile + std::string("__-*-__") + provider
       );
              }

              if (providers.size() == 1) {
                selectedProvider = providers[0];
              }
              else {
                srand(time(NULL));
                providerRandomId = rand() % (providers.size());
                selectedProvider = providers[providerRandomId];
              }

              log = std::string("Selected provider: ") + selectedProvider;
              logQueue->Enqueue( logFile + std::string("__-*-__") + log );

              free(db_zones);

              log = std::string("Creating server.");

              server = CreateServer(selectedProvider, token);
              server->setZone(zone);
              server->setServerName(severName);
              log = std::string("Server type: ") + server->serverType();
              logQueue->Enqueue( logFile + std::string("__-*-__") + log );

              curlLock->getLock();

              //server->create();

              curlLock->releaseLock();

              log = std::string("Server created.");
              logQueue->Enqueue( logFile + std::string("__-*-__") + log );

              free(server);

            }
          }



        }// if ( serverRequest->isCorrect() )
        else{
          free(serverRequest);
          log = std::string("Server resquest incorrect.");
          logQueue->Enqueue( logFile + std::string("__-*-__") + log );
        }

    */
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

  unsigned int killed_queues = 0;

  while ( killed_queues != 2 ) // There are only 2 queues
  {
    if ( ! processerLogQueue.empty() )
    {
      memoryLock.getLock();
      log = processerLogQueue.Dequeue();
      memoryLock.releaseLock();
      if (*log != kill_yourself)
      {
        writeLog( logFileProcesser, log);

        memoryLock.getLock();
        log.reset();
        memoryLock.releaseLock();
      }
      else
      {
        killed_queues++;
      }
    }
    if ( ! managerLogQueue.empty() )
    {
      memoryLock.getLock();
      log = managerLogQueue.Dequeue();
      memoryLock.releaseLock();
      if (*log != kill_yourself)
      {
         writeLog( logFileManager, log);

         memoryLock.getLock();
         log.reset();
         memoryLock.releaseLock();
      }
      else
      {
        killed_queues++;
      }
    }
    usleep(1000);
  }
}

bool writeLog(std::string path,
              boost::shared_ptr<std::string> data) {

  time_t *t;
  struct tm *now;

  memoryLock.getLock();
  t = new time_t(std::time(0));
  now = std::localtime(t);

  //boost:i:shared_ptr<std::string> log(data);
  memoryLock.releaseLock();

  std::ofstream outfile;

  memoryLock.getLock();
  outfile.open( path.c_str(), std::ios_base::app);
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

  outfile.close();
  delete(t);
  memoryLock.releaseLock();
  return true;
}
