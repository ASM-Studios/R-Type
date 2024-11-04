#!/bin/bash

BUILD_FOLDER=cmake-build-debug

if [ ! -d $BUILD_FOLDER ]; then
    mkdir $BUILD_FOLDER
fi
if  command -v ctags >/dev/null; then
    ctags -R server shared client
fi
cd $BUILD_FOLDER
cmake --preset=debug ..
cmake --build .
