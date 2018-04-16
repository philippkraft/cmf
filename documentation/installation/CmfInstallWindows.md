@page cmfInstallWindows

[back...](@ref cmfInstall)

# Installing cmf on Windows for Python 2.7  

This is the simplest way to install cmf on Windows. It shows one path,
that should be suitable for most computers with a Windows system.
Alternatives are listed below

  - Get a recent winpython distribution from
    (<http://sourceforge.net/projects/winpython/>). Today the 64bit
    variant should run on most computers. When in doubt - use 32bit, it
    fits always.
  - "Install" it to any directory on your computer, eg. C:\\Apps
  - Download cmf - Binary for Python 2.7 32 bit from CmfDownload
  - Open C:\\Apps\\Winpython2.732bit in your file explorer and double
    click on the "Winpython Control Panel.exe"
  - In that program, click on a "Add packages" and add the downloaded
    cmf
  - Click install, and here you go...

If you already have an existing Python installation you can also use:

    pip install cmf

### More choice

What you really need to install cmf is a running python 2.7 interpreter
with the `numpy` package. Having other packages from the so called
`scipy-Stack` available, like `scipy`, `matplotlib`, `pandas` is
fine. The best way to get the scipy stack is to install a scientific
python distribution. winpython is one choice, but Anaconda and
python(x,y) are others and they work equally fine. But please follow the
instructions to add packages to these distributions, there are some
traps around.

### Adding Geometry 

Just get the shapely binary from:

<http://www.lfd.uci.edu/~gohlke/pythonlibs/#shapely>

and install it with the pacakge manager of your python distribution. it
is the Winpython Control Panel fro winpython.

## Compile cmf from source for python 2.7 

If you want to use always the most recent version of cmf or if you like
to make changes to the C++ source files of cmf, you need to compile the
C++ files to machine language.

'''Note''': Compiling C++ code on Windows is not simple

To compile sources, you need a compiler - the right compiler. Python 2.7
for Windows is compiled with Visual Studio 2008, and you need the same
compiler. However, you cannot buy this compiler and development tool
anymore, because it is to old. But Microsoft gives the compiler without
the integrated development environment (IDE) away for free.

### Get the right compiler and other prerequisites

Download the right C++ compiler (called Windows SDK) here: [Microsoft
Visual C++ Compiler for
Python 2.7](https://www.microsoft.com/download/details.aspx?id=44266)

And install it. To use the compiler you might need to tweak it a bit, to
have the compiler working seamlessly with Python. Save the following
batch file to as `C:\\Program Files\\Microsoft Visual Studio
9.0\\VC\\vcvarsall.bat`

    @echo off
    REM Calls the right environment depending on your architecture
    if "%1x"=="x86x" "%~dp0\bin\vcvars32.bat"
    if "%1x"=="amd64x" "%~dp0\bin\vcvars64.bat"

Next you need to make the compiler a bit more "modern":

  - Download and install the boost library (<http://www.boost.org>),
    headers only is fine.
  - Create an environmental variable called BOOSTDIR pointing to the
    boost installation directory.

### Compile

  - Get the cmf source code from CmfDownload and unzip it on your hard
    drive
  - Open a command line and navigate to the cmf source directory

Compiling cmf and installing it for your computer is than as simple as
typing in a command line:

'''Note''': Do not test the intallation in the source code folder. The
compiled library will not be found.

    python setup.py install

# Python 3.5

## Without geometry 

This is the simplest way to install cmf on Windows. It shows one path,
that should be suitable for most computers with a Windows system.
Alternatives are listed below

  - Get a recent winpython distribution from
    (<http://sourceforge.net/projects/winpython/>). Today the 64bit
    variant should run on most computers. When in doubt - use 32bit, it
    fits always.
  - "Install" it to any directory on your computer, eg. C:\\Apps
  - Download cmf - Binary for Python 3.5 32 bit from CmfDownload
  - Open C:\\Apps\\Winpython2.732bit in your file explorer and double
    click on the "Winpython Control Panel.exe"
  - In that program, click on a "Add packages" and add the downloaded
    cmf
  - Click install, and here you go...

If you already have an existing Python installation you can also use:

    pip install cmf

## Adding Geometry 

Just get the shapely binary from:

<http://www.lfd.uci.edu/~gohlke/pythonlibs/#shapely>

and install it with the pacakge manager of your python distribution. Use
again the Winpython Control Panel for winpython.

## Compile cmf for Python 3.5 

Compiling cmf for Python 3.5 works in Windows. However, there is no
established procedure by now. The main point is, that Python 3.5 is
compiled with Visual Studio 2015, which is actually available and for
free (no money, no freedom) available for private persons, academic
institutions and small companies. It should also work with: [Visual C++
compiler 2015](http://landinghub.visualstudio.com/visual-cpp-build-tools).
You can find more references on how to compile C and C++ extensions
(like cmf) for Python on Windows
[here](https://wiki.python.org/moin/WindowsCompilers) including links to
the Microsoft Compiler without Visual Studio

@author florianjehn, version: 9 Fri Nov 17 11:30:03 2017
