#!/usr/bin/env bash

ENEK_ROOT_DIR="$(realpath "$(dirname "$(realpath "${BASH_SOURCE[0]}")")"/..)"
SOURCE_PATH="$(realpath "${BASH_SOURCE[1]}")"
SOURCE_PATH_RELATIVE="$(realpath --relative-to="$ENEK_ROOT_DIR" "$SOURCE_PATH")"
PS4='+$SOURCE_PATH_RELATIVE:$LINENO: '
if test -t 1 && tput setaf 1 >&/dev/null; then
    if test "$(tput colors)" == 256; then
        PS4='$(tput setaf 10)'$PS4'$(tput sgr0)'
    else
        PS4='$(tput setaf 2)'$PS4'$(tput sgr0)'
    fi
fi
set -ex

function print_error_message ()
{
    if test -t 2 && tput setaf 1 >&/dev/null; then
        if test "$(tput colors)" == 256; then
            echo "$(tput setaf 9)$1$(tput sgr0)" >&2
        else
            echo "$(tput setaf 1)$1$(tput sgr0)" >&2
        fi
    else
        echo "$1" >&2
    fi
}
