#!/bin/bash

# Move to test directory, to avoid importing the source cmf package
# instead of the installed cmf

cd test
# exit code
ec=0
for testfile in *.py
do
    pkg=${testfile%.*}
    python -m unittest -v $pkg
    # current exit code
    cec=$?
    ec=$(( cec > ec ? cec : ec ))
done

cd ..
echo test.sh exits with $ec
exit $ec
