#!/usr/bin/env bash

THIS_DIR="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
. $THIS_DIR/../util/common.sh

mkdir -p "$1"
cd "$1"
cmake -j $(nproc) -DCMAKE_BUILD_TYPE=Debug "$ENEK_ROOT_DIR"
make -j $(nproc)
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH:+$LD_LIBRARY_PATH:}/usr/local/lib
make -j $(nproc) test ARGS='--output-on-failure --gtest_catch_exceptions=0' VERBOSE=1
