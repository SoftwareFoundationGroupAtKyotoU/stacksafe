#!/bin/bash

if [[ "$1" == -i ]]; then
  cmd=ccmake
else
  cmd=cmake
fi

mkdir -p build
cd build
eval "$cmd" .. -GNinja || exit $?
ninja || exit $?
GTEST_COLOR=1 ctest -V
