REM Gets and makes the dependencies for the new sparese CVodeIntegrator
@echo off

SET CMFDIR=%~dp0..
echo CMFDIR= %CMFDIR%
SET KLU_LIB_DIR=%CMFDIR%\lib
SET SND_LIB_DIR=%CMFDIR%\lib


if "x%1x"=="xklux" goto klu
if "x%1x"=="xsundialsx" goto sundials

echo "BUILD ALL"
REM Make KLU

REM For KLU & sundials build in Windows see:
REM http://my-it-notes.com/2013/01/how-to-build-suitesparse-under-windows-using-visual-studio/
REM https://github.com/jlblancoc/suitesparse-metis-for-windows
REM https://dmerej.info/blog/post/cmake-visual-studio-and-the-command-line/
REM https://ninja-build.org/

:klu
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


if "x%1x"=="xklux" goto end

:sundials

ECHO Install sundials into %SND_LIB_DIR%

SET SND_SRC=%SND_LIB_DIR%\src\sundials
SET SND_URL="https://github.com/philippkraft/sundials"

RD /Q /S %SND_SRC%
mkdir %SND_SRC%
git clone %SND_URL% %SND_SRC%
RD /Q /S %SND_SRC%\.git
RD /Q /S %SND_SRC%\build
mkdir %SND_SRC%\build
cd %SND_SRC%\build

REM For static libraries see: http://sundials.2283335.n4.nabble.com/Visual-Studio-linker-errors-when-building-against-Sundials-static-library-td4654008.html

cmake .. -G "NMake Makefiles" ^
 -DBUILD_ARKODE=OFF -DBUILD_CVODES=OFF -DBUILD_IDA=OFF -DBUILD_IDAS=OFF -DBUILD_KINSOL=OFF ^
 -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=%SND_LIB_DIR% ^
 -DEXAMPLES_INSTALL=OFF ^
 -DKLU_ENABLE=ON -DKLU_LIBRARY_DIR=%KLU_LIB_DIR%/lib -DKLU_INCLUDE_DIR=%KLU_LIB_DIR%/include/suitesparse ^
 -DEXAMPLES_ENABLE_CXX=OFF -DEXAMPLES_ENABLE_C=OFF -DEXAMPLES_INSTALL=OFF ^
 -DOPENMP_ENABLE=ON -DCMAKE_BUILD_TYPE=Release

nmake
nmake install

cd %CMFDIR%

:end

echo "Finished external solvers"