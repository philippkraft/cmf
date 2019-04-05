REM Gets and makes the dependencies for the new sparese CVodeIntegrator
@echo off

SET CMFDIR=%~dp0..
echo CMFDIR= %CMFDIR%
SET LIB_DIR=%CMFDIR%\lib

SET CMF_SRC=%CMFDIR%\cmf\cmf_core_src

mkdir %CMFDIR%\build
cd %CMFDIR%\build
cmake .. -DCMAKE_BUILD_TYPE=Release -G"NMake Makefiles"
nmake
nmake install
cd %CMFDIR%
