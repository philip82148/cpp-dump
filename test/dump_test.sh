#!/bin/sh -eux

g++ ./test/dump_test.cpp -std=c++17 -o ./a.out
./a.out
diff -q ./dump_test.log ./test/dump_test_log.txt
