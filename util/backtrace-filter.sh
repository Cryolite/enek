#!/usr/bin/env bash

(while IFS= read -r line; do
   if echo "$line" | grep -Eq '^[[:space:]]*[[:digit:]]+# 0x[[:xdigit:]]+ in '; then
     # Backtrace from Boost.Stacktrace
     header="$(echo "$line" | sed -e 's/\(^[[:space:]]*[[:digit:]]*# \)0x[[:xdigit:]]* in .*/\1/')"
     addr="$(echo "$line" | sed -e 's/^[[:space:]]*[[:digit:]]*# \(0x[[:xdigit:]]*\) in .*/\1/')"
     file="$(echo "$line" | sed -e 's/^[[:space:]]*[[:digit:]]*# 0x[[:xdigit:]]* in \(.*\)/\1/')"
     if test -x "$file"; then
       addr2line="$(addr2line -e "$file" -f "$addr" | tr '\n' ':')"
       func="$(echo "$addr2line" | cut -d ':' -f 1 | c++filt)"
       file="$(echo "$addr2line" | cut -d ':' -f 2)"
       line="$(echo "$addr2line" | cut -d ':' -f 3)"
       if test xxx"$func"xxx = 'xxx??xxx' -o xxx"$file"xxx = 'xxx??xxx'; then
         echo "$line"
       else
         echo "$header$func at $file:$line"
       fi
     else
       echo "$line"
     fi
   elif echo "$line" | grep -Eq '^[[:space:]]*#[[:digit:]]+[[:space:]]+0x[[:xdigit:]]+ in '; then
     # Backtrace from AddressSanitizer
     head="$(echo "$line" | sed -e 's/^\([[:space:]]*#[[:digit:]]*[[:space:]]*\)0x[[:xdigit:]]* in .* ([^)]*+0x[[:xdigit:]]*)/\1/')"
     addr="$(echo "$line" | sed -e 's/^[[:space:]]*#[[:digit:]]*[[:space:]]*\(0x[[:xdigit:]]*\) in .* ([^)]*+0x[[:xdigit:]]*)/\1/')"
     func="$(echo "$line" | sed -e 's/^[[:space:]]*#[[:digit:]]*[[:space:]]*0x[[:xdigit:]]* in \(.*\) ([^)]*+0x[[:xdigit:]]*)/\1/')"
     file="$(echo "$line" | sed -e 's/^[[:space:]]*#[[:digit:]]*[[:space:]]*0x[[:xdigit:]]* in .* (\([^)]*\)+0x[[:xdigit:]]*)/\1/')"
     addr2="$(echo "$line" | sed -e 's/^[[:space:]]*#[[:digit:]]*[[:space:]]*0x[[:xdigit:]]* in .* ([^)]*+\(0x[[:xdigit:]]*\))/\1/')"
     if test -x "$file"; then
       addr2line="$(addr2line -e "$file" -f "$addr" | tr '\n' ':')"
       func="$(echo "$addr2line" | cut -d ':' -f 1 | c++filt)"
       file="$(echo "$addr2line" | cut -d ':' -f 2)"
       line="$(echo "$addr2line" | cut -d ':' -f 3)"
       if test xxx"$func"xxx = 'xxx??xxx' -o xxx"$file"xxx = 'xxx??xxx'; then
         echo "$line"
       else
         echo "$header$func at $file:$line"
       fi
     else
       echo "$line"
     fi
   else
     echo "$line"
   fi
 done)
