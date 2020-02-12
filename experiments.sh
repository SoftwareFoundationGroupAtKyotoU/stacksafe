#!/bin/bash

test "$1" || exit $?
TARGET_DIR="$1"

setup() {
    mkdir -p build
    pushd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make
    popd
}

setup >&2
for ll in $(find "$TARGET_DIR" -name '*.ll' | sort); do
    echo "$ll" >&2
    opt -analyze -load build/stacksafe.so -stacksafe "$ll"
done
