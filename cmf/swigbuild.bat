@echo off
:swig
if %1x==buildx goto build
if %1x==copyx goto copy_pyd
echo create SWIG wrapper as cmf_swig.cxx
swig -Wextra -w512 -python -castmode -O -c++ -o cmf_swig.cxx cmf.i 

if %ERRORLEVEL%  NEQ 0 goto fine

echo delete cmf_wrap.cxx
if EXIST cmf_wrap.cxx del cmf_wrap.cxx
echo push annotated cmf_swig.cxx into cmf_wrap.cxx
echo /* Created with swigbuild.bat at %date% %time% */ >> cmf_wrap.cxx
echo #pragma warning(push) >> cmf_wrap.cxx
echo #pragma warning (disable : 4244) >> cmf_wrap.cxx
echo #ifndef _CONSOLE >> cmf_wrap.cxx
type cmf_swig.cxx >> cmf_wrap.cxx
echo #endif /* !_CONSOLE */ >> cmf_wrap.cxx
echo #pragma warning(pop) >> cmf_wrap.cxx
echo delete now obsolete cmf_swig.cxx
del cmf_swig.cxx

if %1x==swigx goto fine
if %1x==swigcopyx goto copy_py
:build
devenv ..\cmf.sln /build dllpy26
if %ERRORLEVEL%  NEQ 0 goto fine

:copy_pyd
copy ..\dllpy26\_cmf_core.pyd pycmf\src\cmf\_cmf_core.pyd /Y
:copy_py
copy cmf_core.py pycmf\src\cmf\cmf_core.py /Y
echo copied the extension module to the PyDev workspace
goto fine
:fine
echo Done...
