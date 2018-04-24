#!/bin/sh
export DYLD_LIBRARY_PATH=~/Desktop/malloc/:DYLD_LIBRARY_PATH
cd raw_user_programs
./debug_first.sh
./first
cd ..
exit 0
