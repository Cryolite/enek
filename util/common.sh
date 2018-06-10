#!/usr/bin/env bash

function realpath ()
{
  local script="import os.path; print(os.path.realpath('$1'))"
  if type -t python >/dev/null; then
    python -c "$script"
  elif type -t python2 >/dev/null; then
    python2 -c "$script"
  elif type -t python3 >/dev/null; then
    python3 -c "$script"
  else
    realpath "$1"
  fi
}

function relpath ()
{
  local script="import os.path; print(os.path.relpath('$1', '${2:-$PWD}'))"
  if type -t python >/dev/null; then
    python -c "$script"
  elif type -t python2 >/dev/null; then
    python2 -c "$script"
  elif type -t python3 >/dev/null; then
    python3 -c "$script"
  else
    realpath --relative-to="${2:-$PWD}" "$1"
  fi
}

ENEK_ROOT_DIR="$(realpath "$(dirname "$(realpath "${BASH_SOURCE[0]}")")"/..)"
SOURCE_PATH="$(realpath "${BASH_SOURCE[1]}")"
SOURCE_PATH_RELATIVE="$(relpath "$SOURCE_PATH" "$ENEK_ROOT_DIR")"
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
