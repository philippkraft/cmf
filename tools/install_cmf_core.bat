@echo off
REM Gets and makes the dependencies for the new sparse CVodeIntegrator
REM Check if run nmake is in the path
(
    nmake /? >NUL 2>NUL
) || (
    echo NMAKE.exe not found, are you running a Visual Studio Developer Command Prompt?
    exit /b 1
)

SET CMFDIR=%~dp0..
echo CMFDIR= %CMFDIR%
SET LIB_DIR=%CMFDIR%\lib

SET CMF_SRC=%CMFDIR%\cmf\cmf_core_src

mkdir %CMFDIR%\build\cmf_core
cd %CMFDIR%\build\cmf_core
cmake %CMF_SRC% -DCMAKE_BUILD_TYPE=Release -G"NMake Makefiles"
nmake
nmake install
cd %CMFDIR%
