#!/bin/bash

mkdir -p build
pushd build
if [[ "$1" == -i ]]; then
    ccmake .. -GNinja
    exit
fi
cmake .. -GNinja || exit $?
ninja || exit $?
if [[ -z "$1" ]]; then
    GTEST_COLOR=1 ctest -V
    exit
fi
popd
opt -analyze -load=build/stacksafe.so -stacksafe "$1"
