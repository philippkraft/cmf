@echo off
echo delete old .pyd
python setup.py clean --all
echo build cmf
python setup.py build_ext swig
if ERRORLEVEL 1 goto error
python setup.py build_py swig -c -O2 -f
if ERRORLEVEL 1 goto error
copy build\lib.win-amd64-3.7\cmf\_cmf_core.cp37-win_amd64.pyd cmf /Y

if not "x%1x"=="xinstallx" goto end
python setup.py install

if not "x%2x"=="xtestx" goto end
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