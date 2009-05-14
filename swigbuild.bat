@echo off
:swig
if %1==buildonly goto build
if %1==copy goto copy
swig -Wextra -w512 -macroerrors -python -castmode -O -c++ -v -outdir ..\dllpy25 -o cmf_wrap.cxx cmf.i
echo #pragma warning(push) >> cmf_swig.cxx
echo #pragma warning (disable : 4244) >> cmf_swig.cxx
echo #ifndef _CONSOLE >> cmf_swig.cxx
type cmf_wrap.cxx >> cmf_swig.cxx
echo #endif /* !_CONSOLE */ >> cmf_swig.cxx
echo #pragma warning(pop) >> cmf_swig.cxx
del cmf_wrap.cxx
ren cmf_swig.cxx cmf_wrap.cxx
if %1==swigonly goto fine
:build
devenv ..\cmf.sln /build dllpy25
:copy
copy ..\dllpy25\_cmf_core.pyd pyCMF\src\cmf\_cmf_core.pyd /Y
copy ..\dllpy25\cmf_core.py pyCMF\src\cmf\cmf_core.py /Y
echo copied the extension module to the PyDev workspace
:fine
echo Done...
