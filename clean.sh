#!/bin/bash

if [ -z "$CLIENT" ]; then
    CLIENT=r-type_client
fi

if [ -z "$SERVER" ]; then
    SERVER=r-type_server
fi

rm -rf cmake-build-release
rm -rf cmake-build-debug
rm -rf vgcore*
rm -rf $CLIENT
rm -rf $SERVER