#!/bin/bash -eux

args_array=(
    "dump 160 4"
    "dump_narrow 20 4"
    "dump_wide 4000 4"
    "dump_shallow 4000 0"
)

mkdir -p log
mkdir -p build
make main=./dump_test.cpp out=./build/dump_test.out

for args in "${args_array[@]}"; do
    args=($args)
    basename="${args[0]}"
    width="${args[1]}"
    depth="${args[2]}"

    ./build/dump_test.out $width $depth 0 2>"./log/${basename}.log"
    diff -q "./log/${basename}.log" "./txt/${basename}.txt"

    ./build/dump_test.out $width $depth 1 2>"./log/${basename}.log"
    sed -i "s/\x1B\[[^m]*m//g" "./log/${basename}.log"
    diff -q "./log/${basename}.log" "./txt/${basename}.txt"
done
