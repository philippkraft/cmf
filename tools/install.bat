@echo off
if not "x%1x"=="xdoxyx" goto build
echo doxygen
doxygen tools\Doxyfile
python tools\doxy2swig.py doxy\xml\index.xml cmf\cmf_core_src\docstrings-doxy.i
python tools\docstrings2rest.py cmf\cmf_core_src\docstrings-doxy.i >cmf\cmf_core_src\docstrings.i
echo Created rest complient docstrings in docstrings.i 
:build
if not "x%1x"=="xswigx" goto install
call build.bat
if ERRORLEVEL 1 goto error
:install
python setup.py install
if ERRORLEVEL 1 goto error

if not "x%1x"=="xdistx" goto end
python setup.py bdist_wheel
if ERRORLEVEL 1 goto error

goto end

:error
echo ************************
echo Installation had error !
echo ************************
EXIT /B 1
:end
echo .