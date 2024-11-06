#!/bin/bash

BUILD_TYPE=$1

if [ -z "$BUILD_TYPE" ]; then
  echo "Usage: $0 <build_type>"
  echo "Example: $0 debug"
  exit 1
fi

BUILD_DIR="cmake-build-$BUILD_TYPE"

if [ ! -d "$BUILD_DIR" ]; then
  echo "Build directory $BUILD_DIR does not exist. Please run the build first."
  exit 1
fi

cd $BUILD_DIR
ctest --output-on-failure
