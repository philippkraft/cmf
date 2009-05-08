import os
print "SWIG it"
#os.execv("c:/programme/swig/swig.exe",('-w+202,+309,+321,+322', '-macroerrors','-python','-c++',
#                                       '-outdir ..\dllpy25','-o cmflib_wrap.cxx','CMFLib.i'))
print "SWIG ok"
header ="""
#pragma warning(push) 
#pragma warning (disable : 4244) 
#ifndef _CONSOLE 
"""
fwrap=file('cmflib_wrap.cxx','r')
fout=file('cmflib_temp.cxx','w')
fout.write(header)
undef_debug="""
#ifdef _DEBUG
#define SWIG_DBG
#undef _DEBUG
#endif
"""
redef_debug="""
#ifdef SWIG_DBG
#define _DEBUG
#undef SWIG_DBG
#endif
"""
for line in fwrap:
    if "#include <Python.h>" in line:
        fout.write(undef_debug)
        fout.write(line)
        fout.write(redef_debug)
    else:
        fout.write(line)
fout.close()
fwrap.close()
os.remove('cmflib_wrap.cxx')
os.rename('cmflib_temp.cxx', 'cmflib_wrap.cxx')