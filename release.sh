#!/bin/bash

BUILD_FOLDER=cmake-build-release

if [ ! -d $BUILD_FOLDER ]; then
    mkdir $BUILD_FOLDER
fi
cd $BUILD_FOLDER
cmake -DCMAKE_BUILD_TYPE=Release .. -G "Unix Makefiles"
cmake --build .
