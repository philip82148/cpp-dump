#!/bin/bash -eux

args_array=(
    "dump_test 160 4"
    "dump_test_narrow 20 4"
    "dump_test_wide 2000 4"
    "dump_test_shallow 2000 0"
)

g++ ./test/dump_test.cpp -std=c++17 -o ./a.out

for args in "${args_array[@]}"; do
    args=($args)
    basename="${args[0]}"
    width="${args[1]}"
    depth="${args[2]}"
    ./a.out "./${basename}.log" $width $depth 0
    diff -q "./${basename}.log" "./test/${basename}_log.txt"
    ./a.out "./${basename}.log" $width $depth 1
    sed -i "s/\x1B\[[^m]*m//g" "./${basename}.log"
    diff -q "./${basename}.log" "./test/${basename}_log.txt"
done
