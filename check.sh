#!/bin/bash

dir=build
exec {LOG}>&2

mkdir -p $dir
pushd $dir >&$LOG
if [[ "$1" == -i ]]; then
    ccmake .. -GNinja
    exit
fi
cmake .. -GNinja >&$LOG || exit $?
if [[ -z "$1" ]]; then
    ninja develop
    exit
else
    ninja >&$LOG || exit $?
    popd >&$LOG
    if [[ "$1" =~ .*\.ll ]]; then
        target="$1"
    else
        target="${1%.c}".ll
        clang -S -emit-llvm -c "$1" -o "$target"
    fi
    opt -analyze -load=$dir/stacksafe.so -stacksafe "$target"
fi
