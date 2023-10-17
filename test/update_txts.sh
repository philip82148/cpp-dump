#!/bin/bash -eux

bash ./dump_test.sh || true
bash ./readme_test.sh || true

for f in ./log/*.log; do
    basename=$(basename $f .log)
    cp $f "./txt/${basename}.txt"
done
