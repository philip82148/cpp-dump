#!/bin/bash -eux

args_array=(
    "dump 160 4"
    "dump_narrow 20 4"
    "dump_wide 2000 4"
    "dump_shallow 2000 0"
)

g++ -std=c++17 -o ./a.out ./test/dump_test.cpp

for args in "${args_array[@]}"; do
    args=($args)
    basename="${args[0]}"
    width="${args[1]}"
    depth="${args[2]}"

    ./a.out $width $depth 0 2>"./test_${basename}.log"
    diff -q "./test_${basename}.log" "./test/txt/test_${basename}.txt"

    ./a.out $width $depth 1 2>"./test_${basename}.log"
    sed -i "s/\x1B\[[^m]*m//g" "./test_${basename}.log"
    diff -q "./test_${basename}.log" "./test/txt/test_${basename}.txt"
done
