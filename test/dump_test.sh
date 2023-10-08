#!/bin/bash -eux

args_array=(
    "dump_test 160 4"
    "dump_test_narrow 20 4"
    "dump_test_wide 2000 4"
    "dump_test_shallow 2000 0"
)

g++ -std=c++17 -o ./a.out ./test/dump_test.cpp

for args in "${args_array[@]}"; do
    args=($args)
    basename="${args[0]}"
    width="${args[1]}"
    depth="${args[2]}"

    ./a.out $width $depth 0 2>"./${basename}.log"
    diff -q "./${basename}.log" "./test/txt/${basename}_log.txt"

    ./a.out $width $depth 1 2>"./${basename}.log"
    sed -i "s/\x1B\[[^m]*m//g" "./${basename}.log"
    diff -q "./${basename}.log" "./test/txt/${basename}_log.txt"
done
