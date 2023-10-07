#!/bin/sh -eux

g++ ./test/dump_test.cpp -std=c++17 -o ./a.out
./a.out 0
diff -q ./dump_test.log ./test/dump_test_log.txt
./a.out 1
sed -i "s/\x1B\[[^m]*m//g" ./dump_test.log
diff -q ./dump_test.log ./test/dump_test_log.txt
