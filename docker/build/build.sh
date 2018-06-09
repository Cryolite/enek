#!/usr/bin/env bash

THIS_DIR="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
. "$THIS_DIR/../../util/common.sh"

docker pull cryolite/gcc:latest
(cd "$THIS_DIR" && docker build -t cryolite/enek-build .)
