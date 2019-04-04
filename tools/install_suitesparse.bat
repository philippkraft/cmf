REM Gets and makes the dependencies for the new sparese CVodeIntegrator
@echo off

SET CMFDIR=%~dp0..
echo CMFDIR= %CMFDIR%
SET KLU_LIB_DIR=%CMFDIR%\lib

REM Get KLU (unofficial github with cmake)
SET KLU_SRC=%KLU_LIB_DIR%\src\klu
ECHO Download KLU sources into %KLU_SRC% from %KLU_URL%
SET KLU_URL=https://github.com/philippkraft/suitesparse-metis-for-windows

mkdir %KLU_SRC%
git clone %KLU_URL% %KLU_SRC%
RD /Q /S %KLU_SRC%\.git
mkdir %KLU_SRC%\build
cd %KLU_SRC%\build

cmake .. -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=%KLU_LIB_DIR% -DBUILD_METIS=OFF -DCMAKE_BUILD_TYPE=Release

nmake
nmake install

cd %CMFDIR%
