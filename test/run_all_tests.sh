#!/bin/bash -eux

for f in ./*_test.sh; do
    $f
done
