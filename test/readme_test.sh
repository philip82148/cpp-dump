#!/bin/bash -eux

for f in ../readme/*.cpp; do
    basename=$(basename $f .cpp)

    make main="../readme/${basename}.cpp" out="./build/${basename}.out"
    ./build/${basename}.out 2>"./log/readme_${basename}.log"

    # Do not remove escape sequences but remove raw addresses.
    if [ "${basename}" = "supports-various-types" ] || [ "${basename}" = "customizable-colors" ] || [ "${basename}" = "no-es" ]; then
        sed -i "s/0x[0-9a-f]*//" "./log/readme_${basename}.log"
    fi
    diff -q "./log/readme_${basename}.log" "./txt/readme_${basename}.txt"
done
