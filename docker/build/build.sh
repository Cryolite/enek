#!/usr/bin/env bash

. ../../util/common.sh

docker pull cryolite/gcc:latest
(cd "$THIS_DIR" && docker build -t cryolite/enek-build .)
