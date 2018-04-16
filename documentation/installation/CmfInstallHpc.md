@page cmfInstallHpc

[back...](@ref cmfInstall)

# Install on a HPC

The main reason to run cmf on a high performance computing cluster like
Skylla is to calibrate a model hence to run the model many times. As the
built-in parallelization of cmf is never as effective as a
parallelization of independent runs, this installation guide never uses
the built in parallelization of cmf. This guide includes the
installation of [spotpy](https://pypi.python.org/pypi/spotpy) a
calibration and uncertanty assesment tool, developed at our institute.
Spotpy is easy to use in a parallel environment using a MPI library like
OpenMPI.

These instructions are valid for the HPC cluster "skylla" at the
Justus-Liebig-University, Giessen. If and how this applies to your HPC
environment is hard to tell. For comparison, first a description of the
properties of skylla affecting the installation:

  - Uses GNU modules to configure environments
  - Has python 2.7 or 3.5 "modules" numpy available
  - Uses the Open Grid Engine (former Sun Grid Engine) as job queue
  - Runs on CentOS 6
  - Has OpenMPI available

## Configure environment

To check modules available use:

    #!sh
    module av

### Python 2.7

Add the follwing modules at start up:

    #!sh
    module initadd gcc python2 numpy GE openmpi/gcc
    module add gcc python2 numpy GE openmpi/gcc

First check your python version:

    #!sh
    python --version

Make sure you are using python 2.7 indeed.

If the version is \<=2.7.8 then get the program `pip` to install more
packages. For higher versions, `pip` is already part of Python

    #!sh
    wget https://bootstrap.pypa.io/get-pip.py
    python get-pip.py --user

In case numpy is not available as a module do

    #!sh
    pip install numpy --user

### Python 3

Add the follwing modules at start up:

At our the super computer of the Justus Liebig University, numpy is part
of the python3 module.

    #!sh
    module initadd gcc python3 GE openmpi/gcc
    module add gcc python3 GE openmpi/gcc

### Serveral "Module Commands"

If you want to switch python version or install or remove other modules,
you can use the following commands:

'''Show''' all installed modules:

    #!sh
    module list

'''Remove modules''' you dont want anymore from modules list. E.g. for
switching from python2 to python3 useing "module rm 'modulename'" (numpy
is not needed anymore becaus ist included in python3)

    #!sh
    module rm python2
    module rm numpy

you also need to '''remove the modules''' from the '''init''' (in case
you put them there)[BR](BR)

So check whats in your init

    #!sh
    cat .bashrc

and, in case, remove

    #!sh
    module initrm python2
    module initrm numpy

Now you got rid of python2 and able to use python3 (remember to use
'''python3''' insteand of python to call python [BR](BR)

and use '''pip3''' instead of pip and dont forget --user ;))

## Compile cmf

Copy the cmf source code to your user directory and unzip it in
/cmf-source

    #!sh
    cd cmf-source
    python setup.py install noopenmp --user
    cd .. # Important! You cannot test your installation from the source folder

And check your cmf installation:

    #!python
    python
    import cmf
    p=cmf.project()
    exit()

## Optional packages

If you want to use statistics the pandas tables or create graphs,
install the following packages. They are also needed by spotpy.

'''Note''': If you like the HPC to prepare graphs during the run using
matplotlib, make sure to do before `pylab` is imported: `import
matplotlib; matplotlib.use('Agg')`. This prevents the creation of a
GUI.

    #!sh
    pip install matplotlib --user
    pip install scipy --user
    pip install pandas --user

### Adding geometry

This is only needed if you want to use the geometry support in cmf. In
most cases you will not need it.

First compile and install libgeos to your local lib directory:

    #!sh
    wget http://download.osgeo.org/geos/geos-3.3.5.tar.bz2
    tar -xjf geos-3.3.5.tar.bz2
    cd geos-3.3.5
    ./configure --prefix=$HOME/.local
    make
    make install
    cd ..

Now you need to point the path to that directory

    #!sh
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/.local/lib
    export PATH=$PATH:$HOME/.local/bin

To make this permanent (on every start up), add these lines to the file
/.bash_profile. You can do that with an editor (eg. nano) or with the
commands:

    #!sh
    echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/.local/lib" >>~/.bash_profile
    echo "export PATH=$PATH:$HOME/.local/bin" >>~/.bash_profile
    pip install shapely --user

## Install spotpy on a HPC

To run spotpy on the HPC you need mpi4py installed. While `pip install
mpi4py` should do, it might still not work. In case it fails, compile
it by yourself (eg with adjusted version numbers):

    #!sh
    wget https://bitbucket.org/mpi4py/mpi4py/downloads/mpi4py-2.0.0.tar.gz
    tar -xzf mpi4py-2.0.0.tar.gz
    cd mpi4py-2.0.0
    python setup.py install --user
    cd ..

Now you can install spotpy

    #!sh
    pip install spotpy --user

@author konrad, version: 11 Thu Aug 17 10:30:22 2017
