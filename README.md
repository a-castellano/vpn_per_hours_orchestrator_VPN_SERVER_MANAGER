# HourlyVPN Manager Node

This repo contains the required software for deploying the HourlyVPN Manager Node. This wiki also contains instructions for deploying the project and how it works.

The backend manages HourlyVPN WebApp requests for managing VPN Servers such as creating then, deleting or modifiying their settings.


## Install project dependencies

### Ubuntu 16.04

Install required packages using apt-get

```
# apt-get install clang lldb git make cmake ansible python-pip curl libssh2-1 libssh2-1-dev libmysqlcppconn7v5 libmysqlcppconn-dev libssh-4 libssh-dev
```

Installing required packages using pip

```
# pip install conan
```

### Debian 8
Install required packages using apt-get

```
# apt-get install clang lldb git make cmake ansible python-pip curl libssh2-1 libssh2-1-dev libmysqlcppconn7 libmysqlcppconn-dev libssh-4 libssh-dev
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

#### Ubuntu 16.04

Using conan we need to specify which compiler is going to be used its version, also we need to specify that we are going tu use  **libstdc++11** as linker.
```
$ cd {{PROJECT_ROOT}}/scripts/vpnServerManager/
$ git checkout {{CURRENT_BRANCH}}
$ conan install -s compiler=gcc -s compiler.version=5.4 -s compiler.libcxx=libstdc++11 .
$ conan install -s compiler=gcc -s compiler.version=5.4 -s compiler.libcxx=libstdc++11  --build bzip2 electric-fence zlib OpenSSL libcurl Boost gmock-cmake cmake-include-guard cmake-forward-cache cmake-header-language cmake-opt-arg-parsing .
```


Using Clang
```
$ export CC=/usr/bin/clang
$ export CXX=/usr/bin/clang++
$ conan install -s compiler=clang -s compiler.version=3.8 -s compiler.libcxx=libstdc++11  --build bzip2 electric-fence zlib OpenSSL libcurl Boost gmock-cmake cmake-include-guard cmake-forward-cache cmake-header-language cmake-opt-arg-parsing .
```

#### Debian 8

Using conan we need to specify which compiler is going to be used its version, also we need to specify that we are going tu use  **libstdc++11** as linker.
```
$ cd {{PROJECT_ROOT}}/scripts/vpnServerManager/
$ git checkout {{CURRENT_BRANCH}}
$ conan install -s compiler=gcc -s compiler.version=4.9 -s compiler.libcxx=libstdc++11 .
$ conan install -s compiler=gcc -s compiler.version=4.9 -s compiler.libcxx=libstdc++11  --build bzip2 electric-fence zlib OpenSSL libcurl Boost gmock-cmake cmake-include-guard cmake-forward-cache cmake-header-language cmake-opt-arg-parsing .
```

Using Clang
```
$ export CC=/usr/bin/clang
$ export CXX=/usr/bin/clang++
$ conan install -s compiler=clang -s compiler.version=3.5 -s compiler.libcxx=libstdc++11  --build bzip2 electric-fence zlib OpenSSL libcurl Boost gmock-cmake cmake-include-guard cmake-forward-cache cmake-header-language cmake-opt-arg-parsing .
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
