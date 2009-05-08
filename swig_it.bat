@echo off
echo SWIG it
swig -Wall -w512 -macroerrors -python -castmode -O -c++ -v -outdir ..\dllpy25 -o cmf_wrap.cxx cmf.i
echo #pragma warning(push) >> cmf_swig.cxx
echo #pragma warning (disable : 4244) >> cmf_swig.cxx
echo #ifndef _CONSOLE >> cmf_swig.cxx
type cmf_wrap.cxx >> cmf_swig.cxx
echo #endif /* !_CONSOLE */ >> cmf_swig.cxx
echo #pragma warning(pop) >> cmf_swig.cxx
del cmf_wrap.cxx
ren cmf_swig.cxx cmf_wrap.cxx
copy ..\dllpy25\cmf.py pyCMF\src\cmf.py
