#!/bin/bash -eux

for f in ./readme/*.cpp; do
    basename=$(basename $f .cpp)

    g++ -std=c++17 -o ./a.out "./readme/${basename}.cpp"
    ./a.out 2>"./readme_${basename}.log"

    # Do not remove escape sequences but remove raw addresses.
    if [ "${basename}" = "supports-various-types" ] || [ "${basename}" = "customizable-colors" ] || [ "${basename}" = "no-es" ]; then
        sed -i "s/0x[0-9a-f]*//" "./readme_${basename}.log"
    fi
    diff -q "./readme_${basename}.log" "./test/txt/readme_${basename}_log.txt"
done
