#!/usr/bin/env bash

SOURCE_DIR="$(dirname "$(readlink -e "${BASH_SOURCE[0]}")")"
. "$SOURCE_DIR/../../util/common.sh"

opts="$(getopt -n "$SOURCE_PATH_RELATIVE" -l cache-from: -- '' "$@")"
eval set -- "$opts"

while test "$#" -ne 0; do
  arg="$1"
  shift
  case "$arg" in
  '--cache-from')
    cache_from="$1"
    shift
    ;;
  '--')
    :
    ;;
  *)
    print_error_message "error: An invalid argument \`$arg'."
    exit 1
    ;;
  esac
done

docker_build_options=()

if test -v cache_from; then
  docker_build_options+=("--cache-from=$cache_from")
fi

docker pull cryolite/gcc:latest
(cd "$SOURCE_DIR" && docker build "${docker_build_options[@]}" -t cryolite/enek-build .)
