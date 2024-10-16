#!/bin/bash

BUILD_FOLDER=cmake-build-windows

if [ ! -d $BUILD_FOLDER ]; then
    mkdir $BUILD_FOLDER
fi
cd $BUILD_FOLDER
cmake --preset=windows-from-linux ..
cmake --build .
