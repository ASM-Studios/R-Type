#!/bin/bash

BUILD_FOLDER=cmake-build-debug

if [ ! -d $BUILD_FOLDER ]; then
    mkdir $BUILD_FOLDER
fi
cd $BUILD_FOLDER
cmake --preset=debug ..
cmake --build .
