#!/usr/bin/env bash

SOURCE_DIR="$(dirname "$(readlink -e "${BASH_SOURCE[0]}")")"
. "$SOURCE_DIR/../../etc/common.sh"

"$SOURCE_DIR/build"
docker run --privileged -v "$ENEK_ROOT_DIR:/work/enek" -e TERM -w /work/enek -it --rm cryolite/enek-devel:latest /bin/bash
