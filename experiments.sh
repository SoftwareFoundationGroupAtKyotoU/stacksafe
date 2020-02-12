#!/bin/bash

TARGET_DIR=coreutils

mkdir -p build
pushd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
popd
for ll in $(find "$TARGET_DIR" -name '*.ll' | sort); do
    log=experiments/$(basename "${ll%.ll}").log
    echo "$ll"
    opt -analyze -load build/stacksafe.so -stacksafe "$ll" >"$log"
done
