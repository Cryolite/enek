#!/usr/bin/env bash

SOURCE_DIR="$(dirname "$(readlink -e "${BASH_SOURCE[0]}")")"
. "$SOURCE_DIR/../util/common.sh"

"$ENEK_ROOT_DIR/bin/test" --build-dir=/work/enek-build -j auto --debug --enable-coverage

coveralls_options=()
if test "$TRAVIS_PULL_REQUEST" != false; then
  coveralls_options+=("--service-pull-request=$TRAVIS_PULL_REQUEST")
fi
"$ENEK_ROOT_DIR/util/coveralls.py" --source-root="$ENEK_ROOT_DIR" --build-root=/work/enek-build --with-gcov=/usr/local/bin/gcov --clear --service-name=travis-ci --service-number="$TRAVIS_JOB_NUMBER" --service-job-id="$TRAVIS_JOB_ID" "${coveralls_options[@]}"
