project(VPN)
cmake_minimum_required(VERSION 3.0)

SET (CMAKE_C_COMPILER             "/usr/bin/gcc")
SET (CMAKE_C_FLAGS                "-Wall -std=c99")
SET (CMAKE_C_FLAGS_DEBUG          "-g -fprofile-arcs -ftest-coverage -O0")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (CMAKE_CXX_COMPILER             "/usr/bin/g++")
SET (CMAKE_CXX_FLAGS                "-Wall -std=c++11")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g --coverage -O0")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

INCLUDE(conanbuildinfo.cmake)

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
INCLUDE_DIRECTORIES(lib/vpn_queue)
INCLUDE_DIRECTORIES(lib/manager_node)
INCLUDE_DIRECTORIES(lib/ansible_handler)

#ADD_EXECUTABLE( main src/test.cpp lib/server_request/ServerRequest.cpp lib/vpn_lock/VPNLock.cpp )
#TARGET_LINK_LIBRARIES( main ${CONAN_LIBS})

ADD_EXECUTABLE( server_request_test tests/server_request_test.cpp lib/server_request/ServerRequest.cpp
          lib/vpn_lock/VPNLock.cpp )
TARGET_LINK_LIBRARIES( server_request_test ${CONAN_LIBS} )

ADD_EXECUTABLE( ansible_test tests/ansible_test.cpp lib/vpn_lock/VPNLock.cpp lib/vpn_queue/VPNQueue.cpp lib/server/Server.cpp
  lib/database_handler/DatabaseHandler.cpp lib/ansible_handler/AnsibleHandler.cpp )
TARGET_LINK_LIBRARIES( ansible_test ${CONAN_LIBS} mysqlcppconn )

ADD_EXECUTABLE( pointer_test src/server_pointer.cpp lib/server_pointer/ServerPointer.cpp )
TARGET_LINK_LIBRARIES( pointer_test ${CONAN_LIBS})

ADD_EXECUTABLE( ssh_test src/ssh_test.cpp lib/ssh_tester/sshTester.cpp )
TARGET_LINK_LIBRARIES( ssh_test ${CONAN_LIBS} ssh)

ADD_EXECUTABLE( VPNManagerDaemon src/VPNManagerDeamon.cpp lib/vpn_lock/VPNLock.cpp lib/vpn_queue/VPNQueue.cpp 
  lib/manager_node/ManagerNode.cpp
  lib/server_request/ServerRequest.cpp lib/server/Server.cpp lib/server/DigitalOceanServer.cpp
  lib/server/VultrServer.cpp lib/server/ServerFactory.cpp lib/database_handler/DatabaseHandler.cpp
  lib/server_pointer/ServerPointer.cpp lib/ssh_tester/sshTester.cpp  lib/ansible_handler/AnsibleHandler.cpp )
TARGET_LINK_LIBRARIES( VPNManagerDaemon ${CONAN_LIBS}  mysqlcppconn ssh )


add_test(NAME server_request COMMAND ./bin/server_request_test )
add_test(NAME ansible_test COMMAND ./bin/ansible_test )

#SETUP_TARGET_FOR_COVERAGE(
#        ansible_coverage
#        bin/ansible_test
#        coverage
#)
