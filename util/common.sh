#!/usr/bin/env bash

ENEK_ROOT_DIR="$(realpath "$(dirname "$(realpath "${BASH_SOURCE[0]}")")"/..)"
THIS_DIR="$(dirname "$(realpath "${BASH_SOURCE[1]}")")"
PS4='+$(realpath --relative-to="$ENEK_ROOT_DIR" "$(realpath "${BASH_SOURCE[0]}")"):$LINENO: '
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
