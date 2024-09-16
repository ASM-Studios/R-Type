#!/bin/bash

BUILD_FOLDER=cmake-build-release

if [ ! -d $BUILD_FOLDER ]; then
    mkdir $BUILD_FOLDER
fi
cd $BUILD_FOLDER
cmake --preset=release -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
