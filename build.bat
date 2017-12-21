@echo off
echo delete old .pyd
python setup.py clean --all
echo build cmf
python setup.py build_ext swig
if ERRORLEVEL 1 goto error
move /Y cmf\cmf_core_src\cmf_core.py cmf
python setup.py build_py -c -O2 -f
if ERRORLEVEL 1 goto error
goto end
:error
echo ************************
echo Installation had error !
echo ************************
EXIT /B 1
:end
echo .