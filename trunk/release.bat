@echo off
echo *************************************
echo Python 3.6x64
echo *************************************
SET OLDPATH=%PATH%
call c:\Apps\python3.6x64path.bat
echo doxygen
python setup.py revision
REM doxygen Doxyfile
python doxy2swig.py doxy\xml\index.xml cmf\cmf_core_src\docstrings-doxy.i
python docstrings2rest.py cmf\cmf_core_src\docstrings-doxy.i >cmf\cmf_core_src\docstrings.i
echo Created rest complient docstrings in docstrings.i 
call build.bat
if ERRORLEVEL 1 goto error
python setup.py install
if ERRORLEVEL 1 goto error
python setup.py sdist
python setup.py bdist_wheel

SET PATH=%OLDPATH%
echo *************************************
echo Python 3.5x64
echo *************************************
call c:\Apps\python3.5x64path.bat
call build.bat
if ERRORLEVEL 1 goto error
python setup.py install
if ERRORLEVEL 1 goto error
python setup.py bdist_wheel

SET PATH=%OLDPATH%
echo *************************************
echo Python 2.7x64
echo *************************************
call c:\Apps\python2.7x64path.bat
call build.bat
if ERRORLEVEL 1 goto error
python setup.py install
if ERRORLEVEL 1 goto error
python setup.py bdist_wheel
SET PATH=%OLDPATH%

echo *************************************
echo Python 2.7x32
echo *************************************
call c:\Apps\python2.7x32path.bat
call build.bat
if ERRORLEVEL 1 goto error
python setup.py install
if ERRORLEVEL 1 goto error
python setup.py bdist_wheel
SET PATH=%OLDPATH%

echo *************************************
echo Python 3.5x32
echo *************************************
call c:\Apps\python3.5x32path.bat
call build.bat
if ERRORLEVEL 1 goto error
python setup.py install
if ERRORLEVEL 1 goto error
python setup.py bdist_wheel
SET PATH=%OLDPATH%

echo *************************************
echo Upload to PyPI
echo *************************************
call c:\Apps\python3.6x64path.bat
twine upload dist\*
SET PATH=%OLDPATH%

goto end
:clean
del build /s /q
goto end
:error
echo ************************
echo Release had error !
echo ************************
:end
echo .