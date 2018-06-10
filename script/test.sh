#!/usr/bin/env bash

SOURCE_DIR="$(dirname "$(readlink -e "${BASH_SOURCE[0]}")")"
. "$SOURCE_DIR/../util/common.sh"

opts="$(getopt -n "$SOURCE_PATH_RELATIVE" -l docker,build-dir:,concurrency:,debug,release,enable-coverage,disable-coverage,enable-asan,disable-asan,enable-ubsan,disable-ubsan,enable-tsan,disable-tsan,enable-assert,disable-assert -o j: -- "$@")"
eval set -- "$opts"

if test -z "$CC"; then
  export CC=/usr/local/bin/gcc
fi
if test -z "$CXX"; then
  export CXX=/usr/local/bin/g++
fi
docker=no
build_dir=''
concurrency=no
build_type=Debug
enable_coverage=''
enable_asan=''
enable_ubsan=''
enable_tsan=''
enable_libstdcxx_debug_mode_in_debug=yes
enable_libstdcxx_debug_mode_in_release=no
enable_assert=''

while test "$#" -ne 0; do
  arg="$1"
  shift
  case "$arg" in
  '--docker')
    docker=yes
    ;;
  '--build-dir')
    if test "$#" -eq 0; then
      print_error_message "error: A logic error."
      exit 1
    fi
    build_dir="$1"
    shift
    ;;
  '-j'|'--concurrency')
    if test "$#" -eq 0; then
      print_error_message "error: A logic error."
      exit 1
    fi
    concurrency="$1"
    shift
    case "$concurrency" in
    'auto')
      concurrency=auto
      ;;
    *)
      if ! echo "$concurrency" | grep -Eq '[1-9][[:digit:]]*'; then
        print_error_message "error: An invalid argument \`$concurrency' for \`--concurrency'."
        exit 1
      fi
      ;;
    esac
    ;;
  '--debug')
    build_type=Debug
    ;;
  '--release')
    build_type=Release
    ;;
  '--enable-coverage')
    enable_coverage=yes
    ;;
  '--disable-coverage')
    enable_coverage=no
    ;;
  '--enable-asan')
    enable_asan=yes
    ;;
  '--disable-asan')
    enable_asan=no
    ;;
  '--enable-ubsan')
    enable_ubsan=yes
    ;;
  '--disable-ubsan')
    enable_ubsan=no
    ;;
  '--enable-tsan')
    enable_tsan=yes
    ;;
  '--disable-tsan')
    enable_tsan=no
    ;;
  '--enable-assert')
    enable_assert=yes
    ;;
  '--disable-assert')
    enable_assert=no
    ;;
  '--')
    ;;
  *)
    print_error_message "error: An invalid argument \`$1'."
    exit 1
    ;;
  esac
done

if test "$docker" = yes; then
  declare -a args_for_docker
  eval set -- "$opts"
  while test "$#" -gt 0; do
    arg="$1"
    shift
    case "$arg" in
    '--docker')
      :
      ;;
    *)
      args_for_docker+=("$arg")
      ;;
    esac
  done
  #"$ENEK_ROOT_DIR/docker/build/build.sh"
  docker pull cryolite/enek-build:latest
  docker run --privileged -v "$ENEK_ROOT_DIR":/work/enek -w /work/enek --rm -it cryolite/enek-build:latest /work/enek/script/test.sh "${args_for_docker[@]}"
  exit $?
fi

case "$concurrency" in
'auto')
  if [ -x '/usr/bin/nproc' ]; then
    concurrency="$(nproc)"
  else
    concurrency=no
  fi
  ;;
'no')
  ;;
*)
esac

declare -a make_options

if test "$concurrency" != no; then
  make_options+=(-j "$concurrency")
fi

declare -a cmake_cache_entry_args

cmake_cache_entry_args+=('-DCMAKE_C_COMPILER:FILEPATH=/usr/local/bin/gcc')

cmake_cache_entry_args+=('-DCMAKE_CXX_COMPILER:FILEPATH=/usr/local/bin/g++')

cmake_cache_entry_args+=("-DCMAKE_BUILD_TYPE:STRING=$build_type")

if test "$enable_coverage" = yes; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_COVERAGE:BOOL=ON")
elif test "$enable_coverage" = no; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_COVERAGE:BOOL=OFF")
elif test -z "$enable_coverage"; then
  :
else
  print_error_message "error: A logic error."
  exit 1
fi

if test "$enable_asan" = yes; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_ASAN:BOOL=ON")
elif test "$enable_asan" = no; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_ASAN:BOOL=OFF")
elif test -z "$enable_asan"; then
  :
else
  print_error_message "error: A logic error."
  exit 1
fi

if test "$enable_ubsan" = yes; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_UBSAN:BOOL=ON")
elif test "$enable_ubsan" = no; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_UBSAN:BOOL=OFF")
elif test -z "$enable_ubsan"; then
  :
else
  print_error_message "error: A logic error."
  eixt 1
fi

if test "$enable_tsan" = yes; then
  if test "$enable_asan" = yes; then
    print_error_message "error: AddressSanitizer and ThreadSanitizer cannot be combined."
    exit 1
  fi
  cmake_cache_entry_args+=("-DENEK_ENABLE_TSAN:BOOL=ON")
elif test "$enable_tsan" = no; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_TSAN:BOOL=OFF")
elif test -z "$enable_tsan"; then
  :
else
  print_error_message "error: A logic error."
  exit 1
fi

if test "$build_type" = Debug; then
  enable_libstdcxx_debug_mode="$enable_libstdcxx_debug_mode_in_debug"
else
  enable_libstdcxx_debug_mode="$enable_libstdcxx_debug_mode_in_release"
fi

if test "$enable_libstdcxx_debug_mode" = yes; then
  cmake_cache_entry_args+=('-DENEK_ENABLE_LIBSTDCXX_DEBUG_MODE:BOOL=ON')
else
  cmake_cache_entry_args+=('-DENEK_ENABLE_LIBSTDCXX_DEBUG_MODE:BOOL=OFF')
fi

if test -d "$ENEK_ROOT_DIR/.git" || git rev-parse --git-dir 2>/dev/null; then
  cmake_cache_entry_args+=("-DENEK_GIT_COMMIT_HASH:STRING=$(git rev-parse HEAD)")
else
  cmake_cache_entry_args+=('-DENEK_GIT_COMMIT_HASH:STRING=')
fi

if test "$enable_assert" = yes; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_ASSERT:BOOL=ON")
elif test "$enable_assert" = no; then
  cmake_cache_entry_args+=("-DENEK_ENABLE_ASSERT:BOOL=OFF")
elif test -z "$enable_assert"; then
  :
else
  print_error_message "error: A logic error."
  exit 1
fi

if test -z "$build_dir"; then
  print_error_message "error: The argument for \`--build-dir' is the empty string."
  exit 1
fi
if test -e "$build_dir" -a '!' -d "$build_dir"; then
  print_error_message "error: Cannot create the directory \`$build_dir' because a file exists."
  exit 1
fi
mkdir -p "$build_dir"
cd "$build_dir"

cmake "${cmake_cache_entry_args[@]}" "$ENEK_ROOT_DIR"
make "${make_options[@]}"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:+$LD_LIBRARY_PATH:}/usr/local/lib"
make "${make_options[@]}" test ARGS='--output-on-failure --gtest_catch_exceptions=0'
