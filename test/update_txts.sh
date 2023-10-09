#!/bin/bash -eux

bash ./test/dump_test.sh || true
bash ./test/readme_test.sh || true

for f in ./*.log; do
    basename=$(basename $f .log)
    mv $f "./test/txt/${basename}.txt"
done
