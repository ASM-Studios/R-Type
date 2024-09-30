#!/bin/bash

if [ ! -d "cmake-build-debug" ]; then
    echo "Folder not found"
fi

./r-type_server $@
