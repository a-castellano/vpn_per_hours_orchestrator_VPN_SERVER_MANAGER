# HourlyVPN Manager Node

This repo contains the required software for deploying the HourlyVPN Manager Node. This wiki also contains instructions for deploying the project and how it works.

The backend manages HourlyVPN WebApp requests for managing VPN Servers such as creating then, deleting or modifiying their settings.


## Install project dependencies

### Debian 8

#### Adding Debian testing repo

Create a file named */etc/apt/sources.list.d/testing.list*
```
deb http://ftp.us.debian.org/debian testing main contrib non-free
```



#### Adding Clang 3.9 repos

Create a file named */etc/apt/sources.list.d/llvm-jessie.list*
```
deb http://apt.llvm.org/jessie/ llvm-toolchain-jessie-3.9 main
deb-src http://apt.llvm.org/jessie/ llvm-toolchain-jessie-3.9 main
```

Install required packages using apt-get

```
apt-get update
apt-get install gcc-6 clang-3.8 lldb-3.8 git make cmake ansible python-pip curl libssh2-1 libssh2-1-dev libmysqlcppconn7v5 libmysqlcppconn-dev libssh-4 libssh-dev
```

Installing required packages using pip

```
# pip install conan
```
## Project settings

## Building the project

### Using conana for searching packages
```
conan search PACKAGE_NAME -r conan.io 
```
### C++ vpnServerManager

#### Debian 8

Using conan we need to specify which compiler is going to be used its version, also we need to specify that we are going tu use  **libstdc++11** as linker.
```
conan install -s compiler=gcc -s compiler.version=4.9 -s compiler.libcxx=libstdc++11 .
conan install -s compiler=gcc -s compiler.version=4.9 -s compiler.libcxx=libstdc++11  --build bzip2 electric-fence zlib OpenSSL libcurl Boost googlemock googletest  .
```

Using Clang
```
sudo ln -s /usr/bin/clang-3.9 /usr/bin/clang
sudo ln -s /usr/bin/clang++-3.9 /usr/bin/clang++
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
conan install -s compiler=clang -s compiler.version=3.5 -s compiler.libcxx=libstdc++11  --build bzip2 electric-fence zlib OpenSSL libcurl Boost googlemock googletest  .
```

### Build

Build for Debug
```
cmake -DCMAKE_BUILD_TYPE=Debug .
make
```
Build for Release
```
cmake -DCMAKE_BUILD_TYPE=Release .
$ make
```

## Cleaning the project
```
$ rm -r CMakeCache.txt cmake_install.cmake conanbuildinfo.cmake conaninfo.txt CTestTestfile.cmake Makefile 
```
## Debuging
```
lldb-3.8 ./bin/VPNManagerDaemon
run PORT_NUMBER
```

## Finding memory leaks using valgind (gcc)
```
 valgrind --tool=memcheck --leak-check=full  ./bin/VPNManagerDaemon PORT_NUMBER
```

## Testing issues

During a test it throwed this error:
```
ElectricFence Exiting: mprotect() failed: Cannot allocate memory
```

This happened because the the logger was not running and memory didn't get liberated.

Only for testing
```
echo 128000 > /proc/sys/vm/max_map_count
```

## TODO

Dockerize this manager
