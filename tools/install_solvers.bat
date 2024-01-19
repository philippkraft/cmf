@echo off
REM Gets and makes the dependencies for the new sparse CVodeIntegrator
REM Check if run nmake is in the path
(
    nmake /? >NUL 2>NUL
) || (
    echo NMAKE.exe not found, are you running a Visual Studio Developer Command Prompt?
    exit /b 1
)

SET TOOLSDIR=%~dp0
SET CMFDIR=%TOOLSDIR%..
SET BUILDDIR=%CMFDIR%\build\extern

if not exist %BUILDDIR% mkdir %BUILDDIR%
cd %BUILDDIR%
cmake %TOOLSDIR% -DCMAKE_BUILD_TYPE=Release -G"NMake Makefiles"
nmake
cd %CMFDIR%

