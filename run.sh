#!/bin/sh

if [ $# -ne 1 ]; then
    echo $0: usage: ./run filename
    return
fi

file=$1

make clean; make
./stl_comp $file
