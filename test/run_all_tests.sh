#!/bin/bash -eux

for f in ./test/*_test.sh; do
    $f
done
