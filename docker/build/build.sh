#!/usr/bin/env bash

SOURCE_DIR="$(dirname "$(readlink -e "${BASH_SOURCE[0]}")")"
. "$SOURCE_DIR/../../util/common.sh"

docker pull cryolite/gcc:latest
(cd "$SOURCE_DIR" && docker build -t cryolite/enek-build .)
