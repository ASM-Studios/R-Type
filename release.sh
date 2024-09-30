#!/bin/bash

BUILD_FOLDER=cmake-build-release

if [ ! -d $BUILD_FOLDER ]; then
    mkdir $BUILD_FOLDER
fi
cd $BUILD_FOLDER
cmake --preset=release "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CXX_COMPILER=g++" "-DCMAKE_CXX_FLAGS=-Wno-unused-parameter" ..
cmake --build .
