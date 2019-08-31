#!/bin/bash

mkdir -p build
pushd build
if [[ "$1" == -i ]]; then
    ccmake .. -GNinja
    exit
fi
cmake .. -GNinja || exit $?
ninja || exit $?
GTEST_COLOR=1 ctest -V
popd
if [[ -n "$1" ]]; then
    opt -analyze -load=build/stacksafe.so -stacksafe "$1"
fi
