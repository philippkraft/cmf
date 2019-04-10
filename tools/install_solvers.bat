REM Gets and makes the dependencies for the new sparese CVodeIntegrator
@echo off
SET TOOLSDIR=%~dp0
SET CMFDIR=%TOOLSDIR%..
SET BUILDDIR=%CMFDIR%\build\extern

mkdir %BUILDDIR%
cd %BUILDDIR%
cmake %TOOLSDIR% -DCMAKE_BUILD_TYPE=Release -G"NMake Makefiles"
nmake
cd %CMFDIR%

