#!/bin/bash

mkdir -p build
pushd build
if [[ "$1" == -i ]]; then
    ccmake .. -GNinja
    exit
fi
cmake .. -GNinja || exit $?
if [[ "$1" == develop ]]; then
    ninja
    ninja develop
    exit
elif [[ "$1" == -v ]]; then
    ninja develop || exit $?
    GTEST_COLOR=1 ctest -V
elif [[ -z "$1" ]]; then
    ninja develop || exit $?
    ctest
else
    ninja || exit $?
    popd
    if [[ "$1" =~ .*\.ll ]]; then
        target="$1"
    else
        target="${1%.c}".ll
        clang -S -emit-llvm -c "$1" -o "$target"
    fi
    opt -analyze -load=build/stacksafe.so -stacksafe "$target"
fi
