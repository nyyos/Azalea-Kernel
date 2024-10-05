#!/bin/sh

nm -nP $1 2>/dev/null | grep '.* [tT] ' | "$(dirname "$0")/gen-symmap.pl" > $2
