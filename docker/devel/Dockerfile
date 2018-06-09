FROM cryolite/enek-build:latest

COPY preseed.txt /root
RUN set -ex; \
  apt-get update; \
  apt-get install -y --no-install-recommends \
    locales \
  ; \
  rm -rf /var/lib/apt/lists/*; \
  rm /etc/localtime; \
  rm /etc/timezone; \
  export DEBIAN_FRONTEND=noninteractive DEBCONF_NONINTERACTIVE_SEEN=true; \
  debconf-set-selections /root/preseed.txt; \
  dpkg-reconfigure tzdata; \
  dpkg-reconfigure locales

# `ncurses-term` resolves an error that occurs when `TERM` environment variable
# is equal to `screen.xterm-256color`.
RUN set -ex; \
  apt-get update; \
  apt-get install -y --no-install-recommends \
    bash-completion \
    emacs \
    git \
    less \
    man-db \
    ncurses-term \
    procps \
    screen \
  ; \
  rm -rf /var/lib/apt/lists/*

RUN set -ex; \
  apt-get update; \
  apt-get install -y --no-install-recommends \
    apt-file \
  ; \
  rm -rf /var/lib/apt/lists/*

RUN set -ex; \
  apt-get update; \
  apt-file update

# Copy `cmake-mode.el` to `/root/.emacs.d/lisp`
RUN set -ex; \
  mkdir -p /root/.emacs.d/lisp; \
  cmake_mode_el="$(find /usr/local/share -name 'cmake-mode.el' | head -n 1)"; \
  cp "$cmake_mode_el" /root/.emacs.d/lisp

COPY .bashrc /root
COPY .toprc /root
COPY .screenrc /root
COPY .emacs /root
