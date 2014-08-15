@echo off
python newversionnumber.py
if not "x%1x"=="xdoxyx" goto build
echo doxygen
doxygen Doxyfile
python doxy2swig.py doxy\xml\index.xml cmf\cmf_core_src\docstrings-doxy.i
python docstrings2rest.py cmf\cmf_core_src\docstrings-doxy.i >cmf\cmf_core_src\docstrings.i
echo Created rest complient docstrings in docstrings.i 
:build
if "x%1x"=="xcleanx" goto clean
echo delete old .pyd
del _cmf_core.pyd /s /q
echo build
python setup.py build_ext swig
if ERRORLEVEL 1 goto error
move /Y cmf\cmf_core_src\cmf_core.py cmf
python setup.py build_py -c -O2 -f
if ERRORLEVEL 1 goto error
python setup.py install
if ERRORLEVEL 1 goto error
goto end
:clean
del build /s /q
goto end
:error
echo ************************
echo Installation had error !
echo ************************
:end
echo .