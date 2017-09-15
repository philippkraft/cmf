@echo off
if not "x%1x"=="xdoxyx" goto build
echo doxygen
doxygen Doxyfile
python doxy2swig.py doxy\xml\index.xml cmf\cmf_core_src\docstrings-doxy.i
python docstrings2rest.py cmf\cmf_core_src\docstrings-doxy.i >cmf\cmf_core_src\docstrings.i
echo Created rest complient docstrings in docstrings.i 
:build
if "x%1x"=="xcleanx" goto clean
call build.bat
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
EXIT /B 1
:end
echo .