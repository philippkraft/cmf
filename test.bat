
REM Move to test directory, to avoid importing the source cmf package
REM instead of the installed cmf

cd test
FOR %%t in (*.py) DO python -m unittest -v %%t
cd ..
