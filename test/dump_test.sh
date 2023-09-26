set -eu

g++ ./test/dump_test.cpp -std=c++17 -o ./a.out
./a.out
diff -q ./dump_test.txt ./test/dump_test.txt
