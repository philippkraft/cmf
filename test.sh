#!/bin/bash

# Move to test directory, to avoid importing the source cmf package
# instead of the installed cmf

cd test

for testfile in *.py
do
    echo $testfile
    python -m unittest -v $testfile
done

cd ..