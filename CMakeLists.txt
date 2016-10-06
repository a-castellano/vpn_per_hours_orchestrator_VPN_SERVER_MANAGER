project(HTTP_test)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -fsanitize=address -g") 

include(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

find_package(Threads)

enable_testing()

INCLUDE_DIRECTORIES(src)

INCLUDE_DIRECTORIES(lib/server_request)
INCLUDE_DIRECTORIES(lib/database_handler)
INCLUDE_DIRECTORIES(lib/log)
INCLUDE_DIRECTORIES(lib/server)
INCLUDE_DIRECTORIES(lib/server_pointer)
INCLUDE_DIRECTORIES(lib/ssh_tester)
INCLUDE_DIRECTORIES(lib/vpn_lock)
INCLUDE_DIRECTORIES(lib/manager_node) 


ADD_EXECUTABLE( main src/test.cpp lib/server_request/ServerRequest.cpp lib/vpn_lock/VPNLock.cpp )
TARGET_LINK_LIBRARIES( main ${CONAN_LIBS})

ADD_EXECUTABLE( server_request_test tests/server_request_test.cpp lib/server_request/ServerRequest.cpp
  lib/vpn_lock/VPNLock.cpp )
TARGET_LINK_LIBRARIES( server_request_test ${CONAN_LIBS} )

ADD_EXECUTABLE( mysql_test src/mysql_test.cpp lib/database_handler/DatabaseHandler.cpp )
TARGET_LINK_LIBRARIES( mysql_test ${CONAN_LIBS} mysqlcppconn )

#ADD_EXECUTABLE( log_test src/log_test.cpp lib/log/Log.cpp lib/log/Logger.cpp )
#TARGET_LINK_LIBRARIES( log_test ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

ADD_EXECUTABLE( server_test src/server_test.cpp lib/server/Server.cpp lib/server/DigitalOceanServer.cpp
  lib/server/VultrServer.cpp lib/server/ServerFactory.cpp )
TARGET_LINK_LIBRARIES( server_test ${CONAN_LIBS})

ADD_EXECUTABLE( pointer_test src/server_pointer.cpp lib/server_pointer/ServerPointer.cpp )
TARGET_LINK_LIBRARIES( pointer_test ${CONAN_LIBS})

ADD_EXECUTABLE( ssh_test src/ssh_test.cpp lib/ssh_tester/sshTester.cpp )
TARGET_LINK_LIBRARIES( ssh_test ${CONAN_LIBS} ssh) 

#ADD_EXECUTABLE( http_test src/http_server_test.cpp lib/server/Server.cpp lib/server/DigitalOceanServer.cpp
#  lib/server/VultrServer.cpp lib/server/ServerFactory.cpp lib/server_request/ServerRequest.cpp
#  lib/database_handler/DatabaseHandler.cpp  lib/server_pointer/ServerPointer.cpp 
#  lib/ssh_tester/sshTester.cpp )
#TARGET_LINK_LIBRARIES( http_test ${CONAN_LIBS}   mysqlcppconn ssh pthread  ) 

#ADD_EXECUTABLE( main_server_test src/main_server.cpp lib/server/Server.cpp lib/server/DigitalOceanServer.cpp
#   lib/server/VultrServer.cpp lib/server/ServerFactory.cpp lib/server_request/ServerRequest.cpp
#   lib/database_handler/DatabaseHandler.cpp lib/log/Log.cpp lib/log/Logger.cpp lib/server_pointer/ServerPointer.cpp
#   lib/ssh_tester/sshTester.cpp )
#TARGET_LINK_LIBRARIES( main_server_test ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT}  mysqlcppconn ssh )

#ADD_EXECUTABLE( socket_test src/socket_server.cpp lib/server/Server.cpp lib/server/DigitalOceanServer.cpp
#  lib/server/VultrServer.cpp lib/server/ServerFactory.cpp lib/server_request/ServerRequest.cpp
#  lib/database_handler/DatabaseHandler.cpp lib/log/Log.cpp lib/log/Logger.cpp lib/server_pointer/ServerPointer.cpp
#  lib/ssh_tester/sshTester.cpp )
#TARGET_LINK_LIBRARIES( socket_test ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT}  mysqlcppconn ssh )

ADD_EXECUTABLE( VPNManagerDaemon src/VPNManagerDeamon.cpp lib/manager_node/ManagerNode.cpp
  lib/server_request/ServerRequest.cpp lib/server/Server.cpp lib/server/DigitalOceanServer.cpp
  lib/server/VultrServer.cpp lib/server/ServerFactory.cpp lib/database_handler/DatabaseHandler.cpp
  lib/server_pointer/ServerPointer.cpp lib/ssh_tester/sshTester.cpp lib/vpn_lock/VPNLock.cpp )
TARGET_LINK_LIBRARIES( VPNManagerDaemon ${CONAN_LIBS}  mysqlcppconn ssh )


add_test(NAME server_request COMMAND ./bin/server_request_test )

