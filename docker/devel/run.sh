#!/usr/bin/env bash

THIS_DIR="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
. "$THIS_DIR/../../util/common.sh"

"$THIS_DIR/build.sh"
docker run --privileged -v "$ENEK_ROOT_DIR:/work/enek" -e TERM="$TERM" -w /work/enek --rm -it cryolite/enek-devel:latest /bin/bash
