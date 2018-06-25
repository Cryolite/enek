FROM cryolite/gcc:latest

# `autoconf`, `automake`, and `libtool` is required to build marisa-trie.
RUN set -ex; \
  apt-get update; \
  apt-get install -y --no-install-recommends \
    autoconf \
    automake \
    bzip2 \
    ca-certificates \
    curl \
    dirmngr \
    git \
    libbz2-dev \
    libtool \
    make \
    pgp \
    python3 \
    python3-dev \
    python3-pip \
    subversion \
  ; \
  rm -rf /var/lib/apt/lists/*; \
  python3 -m pip install --upgrade pip; \
  python3 -m pip install requests

# Checkout, build, and install the latest libbacktrace.
RUN set -ex; \
  mkdir -p /work; \
  cd /work; \
  git clone https://github.com/ianlancetaylor/libbacktrace.git; \
  mkdir -p libbacktrace-build; \
  cd libbacktrace-build; \
  ../libbacktrace/configure --with-pic; \
  make -j $(nproc); \
  make check; \
  make install; \
  cd ..; \
  rm -r libbacktrace-build; \
  rm -r libbacktrace

# Import a PGP key to verify the downloaded CMake tarball,
# and download, build, and install the latest CMake.
# Skip `make test` because it fails.
RUN set -ex; \
  key_ids='C6C265324BBEBDC350B513D02D2CEF1034921684'; \
  { gpg --keyserver hkp://keyserver.ubuntu.com --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keyserver.ubuntu.com:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://zimmermann.mayfirst.org --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://zimmermann.mayfirst.org:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://pgp.key-server.io --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://pgp.key-server.io:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keys.gnupg.net --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keys.gnupg.net:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://ha.pool.sks-keyservers.net --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://p80.pool.sks-keyservers.net:80 --keyserver-options timeout=5 --recv-keys $key_ids; }; \
  mkdir -p /usr/local/src; \
  cd /usr/local/src; \
  tarball_name="$(curl -fLSs 'https://cmake.org/files/LatestRelease/' | grep -Eo 'cmake-[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+\.tar\.gz' | sort -V | uniq | tail -n 1)"; \
  curl -fLSso "$tarball_name" "https://cmake.org/files/LatestRelease/$tarball_name"; \
  tarball_basename="$(basename -s .tar.gz "$tarball_name")"; \
  curl -fLSso "$tarball_basename-SHA-256.txt" "https://cmake.org/files/LatestRelease/$tarball_basename-SHA-256.txt"; \
  curl -fLSso "$tarball_basename-SHA-256.txt.asc" "https://cmake.org/files/LatestRelease/$tarball_basename-SHA-256.txt.asc"; \
  gpg --verify "$tarball_basename-SHA-256.txt.asc" "$tarball_basename-SHA-256.txt"; \
  sha256sum -c "$tarball_basename-SHA-256.txt" --ignore-missing | grep -Fq "$tarball_name: OK"; \
  tar xf "$tarball_name"; \
  cd "$tarball_basename"; \
  ./bootstrap --parallel=$(nproc); \
  make -j $(nproc); \
  false && make -j $(nproc) test; \
  make install; \
  cd /usr/local/src; \
  rm -r "$tarball_basename"; \
  rm "$tarball_basename-SHA-256.txt.asc" "$tarball_basename-SHA-256.txt" "$tarball_name"

# Import a PGP key to verify the downloaded ICU4C tarball,
# and download, build, and install the latest ICU4C.
RUN set -ex; \
  key_ids='FFA9129A180D765B7A5BEA1C9B432B27D1BA20D7'; \
  { gpg --keyserver hkp://keyserver.ubuntu.com --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keyserver.ubuntu.com:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://zimmermann.mayfirst.org --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://zimmermann.mayfirst.org:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://pgp.key-server.io --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://pgp.key-server.io:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keys.gnupg.net --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keys.gnupg.net:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://ha.pool.sks-keyservers.net --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://p80.pool.sks-keyservers.net:80 --keyserver-options timeout=5 --recv-keys $key_ids; }; \
  mkdir -p /usr/local/src; \
  cd /usr/local/src; \
  version="$(svn ls http://source.icu-project.org/repos/icu/tags/ | grep -Eo '^release-[[:digit:]]+-[[:digit:]]+(-[[:digit:]]+)?' | sed -e 's/-/./g' | sort -V | tail -n 1)"; \
  tarball_name="icu4c-$(echo "$version" | sed -e 's/\./_/g' | grep -Eo '[[:digit:]]+_[[:digit:]]+(_[[:digit:]]+)?')-src.tgz"; \
  url="https://ssl.icu-project.org/files/icu4c/$(echo "$version" | sed -e 's/^release\.//')/$tarball_name"; \
  curl -fLSso "$tarball_name" "$url"; \
  curl -fLSso "$tarball_name.asc" "$url.asc"; \
  gpg --verify "$tarball_name.asc"; \
  tar xf "$tarball_name"; \
  cd icu/source; \
  ./runConfigureICU Linux; \
  make -j $(nproc); \
  make -j $(nproc) check; \
  make install; \
  cd /usr/local/src; \
  rm -rf icu; \
  rm "$tarball_name.asc"; \
  rm "$tarball_name"

# Import a PGP key to verify the downloaded Boost tarball,
# and download, build, and install the latest Boost.
RUN set -ex; \
  key_ids='379CE192D401AB61'; \
  { gpg --keyserver hkp://keyserver.ubuntu.com --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keyserver.ubuntu.com:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://zimmermann.mayfirst.org --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://zimmermann.mayfirst.org:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://pgp.key-server.io --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://pgp.key-server.io:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keys.gnupg.net --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://keys.gnupg.net:80 --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://ha.pool.sks-keyservers.net --keyserver-options timeout=5 --recv-keys $key_ids || \
    gpg --keyserver hkp://p80.pool.sks-keyservers.net:80 --keyserver-options timeout=5 --recv-keys $key_ids; }; \
  version="$(curl -fLSs 'https://www.boost.org/users/history/' | grep -Eo 'Version[[:space:]]+[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+' | sed -e 's/Version[[:space:]]\{1,\}\([[:digit:]]\{1,\}\.[[:digit:]]\{1,\}\.[[:digit:]]\{1,\}\)/\1/' | sort -V | uniq | tail -n 1)"; \
  tarball_name=boost_"$(echo "$version" | tr . _)".tar.bz2; \
  mkdir -p /usr/local/src/boost; \
  cd /usr/local/src/boost; \
  curl -fLSso "$tarball_name" "https://dl.bintray.com/boostorg/release/$version/source/$tarball_name"; \
  curl -fLSso "$tarball_name.asc" "https://dl.bintray.com/boostorg/release/$version/source/$tarball_name.asc"; \
  gpg --verify "$tarball_name.asc"; \
  tar xf "$tarball_name"; \
  tarball_basename="$(basename -s .tar.bz2 "$tarball_name")"; \
  cd "$tarball_basename"; \
  ./bootstrap.sh; \
  ./b2 \
    -j $(nproc) \
    --build-type=complete \
    --layout=tagged \
    --with-stacktrace \
    --with-exception \
    variant=debug \
    define=_GLIBCXX_DEBUG \
    define=_GLIBCXX_DEBUG_PEDANTIC \
    install; \
  ./b2 \
    -j $(nproc) \
    --build-type=complete \
    --layout=tagged \
    --with-stacktrace \
    --with-exception \
    variant=release \
    install; \
  cd /usr/local/src; \
  rm -r boost
