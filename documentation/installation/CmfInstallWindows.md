@page cmfInstallWindows Windows installation

[back...](@ref cmfInstall)


# Python 3

## Without geometry 

This is the simplest way to install cmf on Windows. It shows one path,
that should be suitable for most computers with a Windows system.
Alternatives are listed below

  - Get a recent winpython distribution from
    (<http://sourceforge.net/projects/winpython/>). Today the 64bit
    variant should run on most computers.
  - "Install" it to any directory on your computer, eg. C:\\Apps 
    
Open the WinPython Command Prompt and type

    pip install cmf

## Adding Geometry 

Just get the shapely binary from:

<http://www.lfd.uci.edu/~gohlke/pythonlibs/#shapely>

and install it with the pacakge manager of your python distribution. Eg. use
 the Winpython Control Panel for winpython.

## Compile cmf for Python 3 with Visual Studio

### Prerequisites

To compile cmf2 you need the following installed on your computer:

- Buildtools for Visual Studio from https://visualstudio.microsoft.com/de/downloads/. Please check 
  Python and Visual Studio compiler version at https://pythondev.readthedocs.io/windows.html
- A Python (>3.6)) installation, preferably a scientific distribution like WinPython or Anaconda,
  including pip
- The CMF source code. Get it using Git or download a ZIP from 
  GitHub.com (https://github.com/philippkraft/cmf)
- Optional: SWIG from swig.org (in the path). If you plan to change the C++ API of CMF you need to
  generate the Python interface with SWIG again.
accessible from  
  

### Installation of requirements

Open a Visual Studio Development Command Prompt and make sure the right python installation
is in the path (eg. python -VV). Navigate to the cmf source code. 
It is a good idea to create a virtual environment (venv) for the compilation

Check if nmake is in the path, if you get an error message you are not using the Visual Studio
Development Command Prompt or you have not installed the Build Tools properly 

    nmake /?

Install the requirements

    pip install -r requirements.txt

Install cmake

    pip install cmake

### One-Step installation of CMF  

Installing and compiling CMF takes some time. The easiest way is to just install cmf from 
the source directory using pip install. In case of problems, it is better to compile CMF 
step by step (see below)

    pip install . -v

The `-v` stands for verbose, to give more output

### Step by step installation of CMF

Compiling and installing of cmf consists of the following steps

1. Download the source code of the solvers in CMF (suitesparse and sundials) and build them
2. Build the CMF C++ library linking the solvers
3. Build the Python extension and link it with the libraries
4. Build the Python tools for CMF
5. Install the whole library in your Python installation
6. Test it

#### Build the solvers (1)

    tools\install_solvers.bat

The source code for the solvers is downloaded to `build\extern`, the built libraries are 
stored in the `lib` directory

#### Build the CMF C++ Library (2)

    tools\install_cmf_core.bat

There are a couple of test executables build together with cmf. 
These executables can be found in the directory `bin`. The static library is together 
with the solver libraries in `lib\lib`

#### Build and link the Python extension (3)

    python setup.py build_ext

If the C++ API has been changed, build the wrapper interface newly with

    python setup.py build_ext swig

#### Build the Python tools

    python setup.py build_py

#### Install the package to the current python version

    python setup.py install

#### Test cmf

Do not fire up a python command prompt and import cmf in the source directory. This will fail.

Either change the directory first or run the cmf tests with

    tools\test.bat

This will take care of the necessary directory change


### Deployment of the compiled version

You can distribute your compiled version to other Windows users by building a binary wheel. 

    python setup.py bdist_wheel

or

    pip wheel .

The .whl file is in the `dist` directory and can be easily shared between computers.

#### Note:
The .whl is ony suitable for the same Python version, architecture (x64/x86) 
and operating system (Windows). If you are distributing a compiled version of CMF,
it is required by the licence to give the receivers of the binary package access to
the source that has been used to compile the binary. In case of an unchanged CMF a 
link to the CMF code repository is sufficient. If you have made changes to the source
code, you are responsible to provide the receiver with your source code, which inherits 
the GNU Public Licence from CMF.

## Compile CMF2 for Python 3 with the Windows Subsystem for Linux (WSL)

Just follow the instructions for installing CMF on Linux