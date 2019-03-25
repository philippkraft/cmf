@echo off
echo delete old .pyd
python setup.py clean --all
echo build cmf
python setup.py build_ext swig
if ERRORLEVEL 1 goto error
python setup.py build_py swig -c -O2 -f
if ERRORLEVEL 1 goto error

if not "x%1x"=="xinstallx" goto end
python setup.py install
cd test
python ..\tools\test.py
cd ..

if ERRORLEVEL 1 goto error
goto end

:error
echo ************************
echo Installation had error !
echo ************************
EXIT /B 1
:end
echo .