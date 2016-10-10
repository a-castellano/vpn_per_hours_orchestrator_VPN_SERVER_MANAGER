# HourlyVPN Manager Node

This repo contains the required software for deploying the HourlyVPN Manager Node. This wiki also contains instructions for deploying the project and how it works.

The backend manages HourlyVPN WebApp requests for managing VPN Servers such as creating then, deleting or modifiying their settings.


## Install project dependencies

### Ubuntu

Install required packages uding apt-get

```
# apt-get install ansible python-pip curl libssh2-1 libssh2-1-dev libmysqlcppconn7v5 libmysqlcppconn-dev libssh-4 libssh-dev 
```

Installing required packages using pip

```
# pip install conan
```

### Debian 8
Install required packages uding apt-get

```
# apt-get install ansible python-pip curl libssh2-1 libssh2-1-dev libmysqlcppconn7 libmysqlcppconn-dev libssh-4 libssh-dev
```

Installing required packages using pip

```
# pip install conan
```
## Project settings

## Building the project


### C++ vpnServerManager

#### Ubuntu 16.04

Using conan we need to specify which compiler is going to be used its version, also we need to specify that we are going tu use  **libstdc++11** as linker.
```
$ cd {{PROJECT_ROOT}}/scripts/vpnServerManager/
$ git checkout {{CURRENT_BRANCH}}
$ conan install -s compiler=gcc -s compiler.version=5.3 -s compiler.libcxx=libstdc++11 .
$ conan install -s compiler=gcc -s compiler.version=5.4 -s compiler.libcxx=libstdc++11  --build bzip2 electric-fence zlib OpenSSL libcurl .
```
Build for Debug
```
cmake -DCMAKE_BUILD_TYPE=Debug .
```
Build for Release
```
cmake -DCMAKE_BUILD_TYPE=Release .
```
```
$ cmake .
$ make
```

Finding memory leaks using valgind
```
 valgrind --tool=memcheck --leak-check=full  ./bin/VPNManagerDaemon PORT_NUMBER
```

## TODO

This node is not going to the multi trhead. It will be an single dockerized node. The more nodes the system needs, the more docker will be deployed.
