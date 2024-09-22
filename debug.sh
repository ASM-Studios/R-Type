#!/bin/bash

BUILD_FOLDER=cmake-build-debug

if [ ! -d $BUILD_FOLDER ]; then
    mkdir $BUILD_FOLDER
fi
cd $BUILD_FOLDER
cmake --preset=debug "-DCMAKE_BUILD_TYPE=Debug" "-DCMAKE_CXX_COMPILER=g++" "-DCMAKE_CXX_FLAGS=-Wno-unused-parameter" ..
cmake --build .
