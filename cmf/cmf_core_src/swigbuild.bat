@echo off
echo create SWIG wrapper as cmf_swig.cxx
swig -Wextra -w512 -w511 -python -keyword -castmode -O -c++ -o cmf_swig.cxx -outdir .. cmf.i 

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

:fine
echo swigbuild finished