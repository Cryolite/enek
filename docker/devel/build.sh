#!/usr/bin/env bash

THIS_DIR="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
. "$THIS_DIR/../../util/common.sh"

if type -t timedatectl >&/dev/null; then
    timezone="$(timedatectl | grep -F 'Time zone:' | sed -e 's/.*Time zone:[[:space:]]\{1,\}\([^[:space:]]\{1,\}\)[[:space:]]\{1,\}.*/\1/')"
elif test -f /etc/timezone; then
    timezone="$(cat /etc/timezone)"
else
    print_error_message 'error: could not obtain the timezone'
    exit 1
fi
timezone_area="$(echo "$timezone" | sed -e 's@/.*@@')"
timezone_location="$(echo "$timezone" | sed -e 's@.*/@@')"
cp "$THIS_DIR/preseed.txt.orig" "$THIS_DIR/preseed.txt"
sed -i -e "s@\(tzdata[[:space:]]\{1,\}tzdata/Areas[[:space:]]\{1,\}select\{1,\}[[:space:]]\{1,\}\).*@\1$timezone_area@" "$THIS_DIR/preseed.txt"
sed -i -e "s@\(tzdata\s\{1,\}tzdata/Zones/\)[^[:space:]]\{1,\}\([[:space:]]\{1,\}select\{1,\}[[:space:]]\{1,\}\).*@\1$timezone_area\2$timezone_location@" "$THIS_DIR/preseed.txt"

"$ENEK_ROOT_DIR/docker/build/build.sh"
(cd "$THIS_DIR" && docker build -t cryolite/enek-devel .)
