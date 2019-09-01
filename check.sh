#!/bin/bash

mkdir -p build
pushd build
if [[ "$1" == -i ]]; then
    ccmake .. -GNinja
    exit
fi
cmake .. -GNinja || exit $?
if [[ "$1" == develop ]]; then
    ninja develop
    exit
elif [[ -z "$1" ]]; then
    ninja develop || exit $?
    GTEST_COLOR=1 ctest -V
else
    ninja || exit $?
    popd
    opt -analyze -load=build/stacksafe.so -stacksafe "$1"
fi
