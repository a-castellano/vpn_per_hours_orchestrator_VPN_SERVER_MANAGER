// ansible_test.cpp
// Álvaro Castellano Vela 19/10/2016

#define BOOST_TEST_MODULE AnsibleTest

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

#include <gmock/gmock.h>
#include <boost/test/unit_test.hpp>

#include <AnsibleHandler.h>
#include <DatabaseHandler.h>
#include <Server.h>
#include <VPNLock.h>
#include <VPNQueue.h>

#define SERVER_TEST_IP "45.32.146.9"

VPNLock  memoryLock;
VPNQueue managerLogQueue;

class ServerMock : public Server {
  public:

    ServerMock(  boost::shared_ptr<std::string> token )
       : Server(token) {}

    MOCK_METHOD0( create, bool());
    MOCK_METHOD0( destroy, bool());
    MOCK_METHOD0( powerOn, bool());
    MOCK_METHOD0( powerOff, bool());

    MOCK_METHOD0( getServerIP, std::string() );
};

BOOST_AUTO_TEST_CASE( ansible_test ) {
  std::string address("paula.es.una.ninja");
  std::string user("vpn");
  std::string password("XLRc3H1y4Db0G4qR630Qk2xPF3D88P");
  std::string database("vpn");

  DatabaseHandler *db;

  boost::shared_ptr<std::string> token;

  AnsibleHandler *ansible;

  token = boost::make_shared<std::string>("KySQUUylQ0WsAjApHtgrwU3HLb0fiFQAEOm4tGIt5KoHZwMYRYyjfDJAA33q");

  ServerMock server(token);

  bool server_created;

  EXPECT_CALL(server, create()).WillRepeatedly(testing::Return(true));
  EXPECT_CALL(server, destroy()).WillRepeatedly(testing::Return(true));
  EXPECT_CALL(server, powerOn()).WillRepeatedly(testing::Return(true));
  EXPECT_CALL(server, powerOff()).WillRepeatedly(testing::Return(true));

  EXPECT_CALL(server, getServerIP()).WillRepeatedly(testing::Return(std::string(SERVER_TEST_IP)));

  server_created = server.create();

  BOOST_CHECK_EQUAL( server_created, true );

  if (server_created)
  {
    BOOST_CHECK_EQUAL( server.getServerIP(), std::string(SERVER_TEST_IP) );

    db = new DatabaseHandler(address, 3306, user, password, database);

    BOOST_CHECK_EQUAL( db->dataIsWellFormed(), true);
    BOOST_CHECK_EQUAL( db->hasError(), false );
    BOOST_CHECK_EQUAL( db->queryTest(), true );

    ansible = new AnsibleHandler(token, db,  server.getServerIP());

    ansible->run();

    delete(ansible);
    delete(db);
  }

  token.reset();
}
