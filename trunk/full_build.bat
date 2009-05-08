@echo Copy CMFLib.py from pyCMF/src to dllpy25
@copy pyCMF\src\CMFLib.py ..\DllPy25\CMFLib.py /y
@echo Compile...
@"%programfiles%\Microsoft Visual Studio 8\Common7\IDE\devenv" ..\CMFLib.sln /build dllpy25
