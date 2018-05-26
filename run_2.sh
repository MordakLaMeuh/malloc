#!/bin/sh
export DYLD_LIBRARY_PATH=/Users/bmickael/Desktop/malloc
export DYLD_INSERT_LIBRARIES="/Users/bmickael/Desktop/malloc/libft_malloc.so"
export DYLD_FORCE_FLAT_NAMESPACE=1
$@

