#!/bin/bash
cd ..
chmod +x autogen.sh
./autogen.sh
./configure
CC=afl-clang-fast AFL_HARDEN=1 AFL_USE_ASAN=1 CFLAGS="-g -w" make 
cd fuzz
AFL_HARDEN=1 afl-clang-fast -fsanitize=address,undefined -fno-sanitize-recover=address,undefined -O2 -o harness harness.c

