@page InstallCheck

[index](@ref tutorial)

# Check the cmf installation

To check your cmf installation, start an [interactive
python](RecommendedSoftwareEnvironment) console and type in the
following:

    #!python
    import cmf
    project = cmf.project()
    cell = project.NewCell(0,0,0,1000)

If you get an error message on the import statement, cmf is not
correctly installed or you have started the console in the source
directory of cmf. Refer to InstallCmf as a guidance.

A typical problem using the binary files in Windows, an error called
something like "Wrong Side-by-Side configuration". This happens by a
wrong Microsoft C++ Runtime environment. For Python 2, you need the 2008
version. Search the web for "vcredist.exe 2008" and add a x64 if you are
using a 64 bit Python.

If you have still problems, ask for guidance by e-mail. Please send the
exact steps you have been taking for installation together with a system
description (OS, Python interpreter, C++ compiler). If you don't get an
error message, you have created your first empty cmf-cell. To read on,
go to [projects and cells](DemoProject)

author: philipp, version: 5 Mon Dec 7 10:04:27 2015
