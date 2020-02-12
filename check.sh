#!/bin/bash

mkdir -p build
pushd build
if [[ "$1" == -i ]]; then
    ccmake .. -GNinja
    exit
fi
cmake .. -GNinja || exit $?
if [[ -z "$1" ]]; then
    ninja develop
    exit
else
    ninja || exit $?
    popd
    if [[ "$1" =~ .*\.ll ]]; then
        target="$1"
    else
        target="${1%.c}".ll
        clang -S -emit-llvm -c "$1" -o "$target"
    fi
    opt -analyze -load=build/stacksafe.so -stacksafe -debug-only=log "$target"
fi
