@page CmfInstallUbuntu

[back...](@ref CmfInstall)

# Install cmf on a Linux with sudo privileges

First you need to make sure that the dependencies of cmf are fullfilled.
It is good practise to get ppackages from your Linux distribution, if
they are available and to get only those packages, that you can't get
from your distribution to install directly in Python. Any package
installed by your OS package manager will be updated by the OS, while
other packages need to get updated by yourself.

I am working mostly with Ubuntu variants, hence i can't say anything
about other Linux package managers. However an internet search like
"apt-get fedora" will help.

## Python 2.7 

You need sudo privileges to install the dependencies for the system. You
will need `python-dev` and `numpy`. Recommended are `matplotlib`,
`scipy`and `pandas`.

### Dependencies of cmf from the Ubuntu package manager

    #!sh
    sudo apt-get install python-dev python-numpy python-scipy python-pandas python-matplotlib

### Get dependencies from the Python Package Index (PyPI)

Follow this steps if you can't get the packages from your OS or if you
do not have sudo privileges. This text assumes you want everything only
installed for yourself and not for other users.

First check your python version:

    #!sh
    python --version

If the version is \<=2.7.8 then get the program `pip` to install more
packages.For higher versions, `pip` is already part of Python

    #!sh
    wget https://bootstrap.pypa.io/get-pip.py
    python get-pip.py --user

The next steps are only needed if you did not used the `apt-get`
commands from above:

    #!sh
    pip install numpy --user
    pip install matplotlib --user
    pip install scipy --user
    pip install pandas --user

### Compile cmf

Get the source code from CmfDownload and unzip it to /cmf-source

    #!sh
    cd cmf-source
    python setup.py install --user
    cd .. # Important! You cannot test your installation from the source folder

or, without multi core processor support (without OpenMP)

    #!sh
    cd cmf-source
    python setup.py install noopenmp --user
    cd .. # Important! You cannot test your installation from the source folder

## Adding geometric support to CMF 

To added geometric features to cmf, you need to install the geos library
and shapely. For the geos library, you need sudo privileges

    #!sh
    wget http://download.osgeo.org/geos/geos-3.2.2.tar.bz2
    tar -xjf geos-3.2.2.tar.bz2
    cd geos-3.2.2
    ./configure
    make
    sudo make install
    cd ..
    pip install shapely --user

## Python 3.5 

Work in progress..

### Adding geometric support to CMF 

@author philipp, version: 4 Thu Jun 23 13:22:29 2016
