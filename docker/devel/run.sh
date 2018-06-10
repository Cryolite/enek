#!/usr/bin/env bash

SOURCE_DIR="$(dirname "$(readlink -e "${BASH_SOURCE[0]}")")"
. "$SOURCE_DIR/../../util/common.sh"

"$SOURCE_DIR/build.sh"
docker run --privileged -v "$ENEK_ROOT_DIR:/work/enek" -e TERM="$TERM" -w /work/enek --rm -it cryolite/enek-devel:latest /bin/bash
