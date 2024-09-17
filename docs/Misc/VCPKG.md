
# How to handle 3rd party dependency ?
## Download and install VCPKG

Download vcpkg

```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
```

Now setup your .zshrc

```sh
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

## Setup a project with VCPKG

Create a new project

```sh
vcpkg new --application
```

## Add dependencies with VCPKG (example with SFML)

First dependency in vcpkg

```sh
vcpkg add port sfml
```

Link dependency in your CMakeLists.txt

```CMake
cmake_minimum_required(VERSION 3.27.4)
project(r-type_client)

include_directories(include)

# ->
find_package(SFML COMPONENTS system window graphics network audio CONFIG REQUIRED)
# <-

set(SOURCE_FILE src/main.cpp)

add_executable(r-type_client ${SOURCE_FILE})

# ->
target_link_libraries(r-type_client sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
# <-
```

## Build and run the project

Create a CMakePresets.json file

```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "debug",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/cmake-build-debug",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      }
    },
    {
      "name": "release",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/cmake-build-release",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```

### To create a debug build:

You can either...

run these two commands
```sh
cmake --preset=debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug
```

or run the [debug.sh](../../debug.sh) script
```sh
./debug.sh
```
### To create a release build:

You can either...

run these two commands
```sh
cmake --preset=release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release
```

or run the [release.sh](../../release.sh) script
```sh
./release.sh
```
