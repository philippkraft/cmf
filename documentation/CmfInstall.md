@page CmfInstall

# Installation instructions

There are numerous ways to install cmf, depending on your operating
system, your Python environment and your choice of what you would like
to do with cmf. One important question is if you want to use geometric
objects in cmf, or not. No tutorial shows the use of these objects and
it is no problem to install geometric objects later.

The following list should be seen as a decision tree pointing you to the
correct installation instructions, but this tree is currently under
construction:

## \[\[CmfInstallWindows|Windows\]\]

Cmf is known to run on Windows XP - 10

### Python 2.7

  - [Keep it simple](CmfInstallWindows#noshapely2.7) (uses winpython
    32bit/64bit)
  - [Use geometry](CmfInstallWindows#shapely2.7) (uses winpython
    32bit/64bit)
  - [Compile cmf by your self](CmfInstallWindows#compile2.7) - only for
    advanced users

### Python 3.5+

  - [Keep it simple](CmfInstallWindows#noshapely3.5) (uses winpython
    32bit/64bit)
  - [Use geometry](CmfInstallWindows#shapely3.5) (uses winpython
    32bit/64bit)
  - [Compile cmf by your self](CmfInstallWindows#compile3.5) - only for
    advanced
users

## \[\[CmfInstallUbuntu|Ubuntu Desktop\]\] and other Linux distributions with sudo privileges

cmf has been used from Ubuntu 10.04 to 16.04. The instructions assume
you have sudo rights on the computer

### Python 2.7

  - [Compile without geometry](CmfInstallUbuntu#noshapely2.7)
  - [Add geometry](CmfInstallUbuntu#shapely2.7)

### Python 3.5+

  - [Compile without geometry](CmfInstallUbuntu#noshapely3.5)
  - [Add geometry](CmfInstallUbuntu#shapely3.5)

## \[\[CmfInstallHpc|Cent OS 6\]\], for high performance computing

### Python 2.7

  - [Compile without geometry](CmfInstallHpc#noshapely2.7)
  - [Add geometry](CmfInstallHpc#shapely2.7)

### Python 3.5

  - [Compile without geometry](CmfInstallHpc#noshapely3.5)
  - [Add geometry](CmfInstallHpc#shapely3.5)

## Mac OS X \>= 10.6 (short instruction)

'''Info''': The developers do now have access to a Mac. We are very
happy about it. The following lines try to give a help how to get cmf
working. However the installation process could be timeintensive and
difficult on some steps, so if there are any troubles, so do not
hesitate to write a mail to Benjamin.Manns@math.uni-giessen.de. We
recommend to install a python3 version from <https://www.python.org/>
and use this version later. If you run the installer, you will have a
pip3.\* and python3.\* version installed. We can not support python2.7
and older for Mac.

'''Note''': Here is a very short instruction which worked for some
users. If it will not work for you, you may want to look in the detailed
instruction

In general, you can just use

``` {.sh}

python3.* setup.py install
```

It should does every step what is done below.

## Mac OS X \>= 10.6 (long version)

#### Install brew and python

  - I repeat my remark from above. Please install Python3 from
    <https://www.python.org/> and use always a python3.\* and pip3.\*
    version. Python2.7 is not supported for Mac.

<!-- end list -->

  - To get cmf run we need a special cpp Framework, called \*Boost\*
    (<http://www.boost.org/>). We check if your Mac has a working
    version of Boost.

Therefor we run the test cpp Script in the root director of the CMF
source Code:

``` {.sh}

clang  -x c++ -E   testboost.cpp 
```

Which gives us a info which version of boost is availabe and where and
if not we get an error so we know that there is an installation
important.

  - An Installation of Boost, if not available, can be done in this
    following two steps:

<!-- end list -->

  - First we want to install brew \[<http://brew.sh/>\] which is a
    Non-Apple but very useful package manager, like ''apt'' from Ubuntu.
    Install brew by opening your Terminal (Search Terminal in your
    Spotlight). Copy and paste following code snippet and press Enter.

<!-- end list -->

``` {.sh}

/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

If there is the question if you want to install Xcode, then allow it.
Otherwise brew will not work properly. Belonging to what you already did
with your Mac, the question might not appear. This whole process takes
some minutes, this is normal.

Now we test if brew was installed fine:

``` {.sh}

brew update
```

After this search for gcc, version 7.x or above:

``` {.sh}

brew search gcc
```

Then download and install gcc:

``` {.sh}

brew install gcc
```

After this we can start with this command in the root command:

``` {.sh}

python setup.py install
```

  - You will see a lots of warning

But if you see a \*build ok\*, thats great:

    Processing cmf-0.1447-py3.5-macosx-10.6-intel.egg
    removing '/Library/Frameworks/Python.framework/Versions/3.5/lib/python3.5/site-packages/cmf-0.1447-py3.5-macosx-10.6-intel.egg' (and everything under it)
    creating /Library/Frameworks/Python.framework/Versions/3.5/lib/python3.5/site-packages/cmf-0.1447-py3.5-macosx-10.6-intel.egg
    Extracting cmf-0.1447-py3.5-macosx-10.6-intel.egg to /Library/Frameworks/Python.framework/Versions/3.5/lib/python3.5/site-packages
    cmf 0.1447 is already the active version in easy-install.pth
    
    Installed /Library/Frameworks/Python.framework/Versions/3.5/lib/python3.5/site-packages/cmf-0.1447-py3.5-macosx-10.6-intel.egg
    Processing dependencies for cmf==0.1447
    Finished processing dependencies for cmf==0.1447
    build ok

But if you have an error in this step please report them to [at
math.uni-giessen.de](Benjamin.Manns).

  - Assuming you passed this step the greater will follow now\! Open
    python and import cmf. It could be occur an error, I don't hope
    that, but it might be. If this is so, please report that to. I guess
    I will have to look by myself in you Mac and see what is going
    wrong.

<!-- end list -->

``` {.py}

import cmf
```

  - If you see no error, then everything is fine, you can start with the
    examples.

author: philipp, version: 32 Thu Dec 14 11:45:46 2017
