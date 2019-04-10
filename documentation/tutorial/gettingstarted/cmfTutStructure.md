@page cmfTutStructure The structure of a cmf program

This tutorial is still under development, sorry

To get a cmf program to do what you want, the order of creating
the building blocks of cmf is of importance.  

## Programming style

Python comes with some syntactic rules how a program works. But there
are also not enforced rules, how a Python program should look like,
the so called [PEP 8 style](https://www.python.org/dev/peps/pep-0008/).
The cmf library does often not follow this style guide, but I wish knew
it when I wrote cmf.    

## Simple CMF - scripts

A simple cmf script is just a batch of commands. The only structures used
are conditions (if) and loops (for / while). Such programs are very
simple to understand and followed, if they are short and every reader will
always be interested in every detail. This type of script is fine for the 
beginner, because the flow of the program is obvious: from top to bottom,
like a normal text. When your program grows beyond 50 to 100 lines, 
take 10 minutes to restructure your program using functions or classes. 

### Module docstring and shebang

If you will ever run your model on a Linux / Unix system, it is a good
idea to start the file with the so called shebang:
~~~~~~~~~~~~~~.py
#! /usr/bin/env python3
~~~~~~~~~~~~~~

Followed by a docstring explaining the purpose of your program:

~~~~~~~~~~~~~~.py
"""
This cmf program is written to demonstrate 
the structure of any other cmf program

:author: philippkraft (my Github user-name)
:date: 2018-5-9 
"""
~~~~~~~~~~~~~~
### Import

To use cmf, the cmf-library must be imported. However, if you like to write
code that is compatible for Python 2 and 3 the first line of your program
is the `from __future__ import` line for Python 2 compatibility. If you need
other libraries / packages in your program, import them also in the beginning.
Avoid `from xxx import *` commands in all scripts.

The import section of a CMF script, that uses `numpy`-arrays and `matplotlib` 
plotting and `datetime`'s objects might look like:

~~~~~~~~~~~~~.py
from __future__ import division, print_function, absolute_import
import numpy as np
from matplotlib import pylab as plt
from datetime import datetime, timedelta
import cmf
~~~~~~~~~~~~~

### Build setup

The setup part of a cmf program is, where the structure of the cmf model is defined.
The structure consists of the storages, boundary conditions and connections and for 
more advanced models also of the cells with soil layers and vegetation.

In the @ref cmfTutBoundary example the setup part is, where the project, the two
storages, the two boundary conditions and the connections are defined:

~~~~~~~~~~~~~.py

p = cmf.project()
W1 = p.NewStorage(name="W1",x=0,y=0,z=0)
W2 = p.NewStorage(name="W2",x=10,y=0,z=0)
q = cmf.LinearStorageConnection(source=W1,target=W2,residencetime=1.0)
b_out = p.NewOutlet(name="Outlet",x=20,y=0,z=0)
b_out_q = cmf.LinearStorageConnection(source=W2,target=b_out,residencetime=2.0)

# Create a Neumann Boundary condition connected to W1
b_in = cmf.NeumannBoundary.create(W1)

~~~~~~~~~~~~~

### Assign driver data and initial values

CMF models are run from an initial condition using forcing (or driver)
data during runtime. The place between defining the structure of
your model and creating a solver is a good place to overwrite
the default initial state (that are not very well documented but
are usually some kind of "empty") and load and assign forcing data.

In our case, the initial condition is the stored volume of `W1` and
`W2` and the forcing data is the flux over the Neumann boundary condition.

~~~~~~~~~~~~~~.py
W1.volume = 1.0
W2.volume = 0.0
b_in.flux = cmf.timeseries(begin = datetime.datetime(2012,1,1), 
                           step = datetime.timedelta(days=1), 
                           interpolationmethod = 0)
for i in range(10):
    # Add 0.0 m3/day for even days, and 1.0 m3/day for odd days
    b_in.flux.add(i % 2)
~~~~~~~~~~~~~~

### Create solver

The solver should only be created **after** the structure is setup, since solvers
in cmf do not own a reference to a project, or what else, but only extract references
to the state variables of a project, a cell or some other owner of state variables. 
This means, if you create a solver and after that, you
are going to extend your project with new storages, the solver knows nothing about these 
and the mass conversation of the model is broken. If you extend your project, eg. in
an interactive environment, just make sure to create a new solver also.

~~~~~~~~~~~~~~~.py
solver = cmf.ImplicitEuler(p, 1e-9)
~~~~~~~~~~~~~~~

### Runtime loop

The solver is used to advance the model in the runtime loop. The runtime loop is
user defined, to allow the user to interact with the running model in any way.
Usages of the runtime loop include:

- Store any data from the model in files or in-memory collections (eg. lists, arrays,
cmf.timeseries, pandas frames etc.)
- Print model progress to screen 
- Visual animations of the model state (see cmf/demo/cmf2d.py)
- Interact with other models as an operator split (@ref publicationList)

To build the runtime loop, the generator method `run` of the solver makes the iteration
over the model time simple:

~~~~~~~~~~~~~~~~~~~~.py
starttime = datetime(2018, 5, 9)
endtime = datetime(2018, 7, 9)
timestep = timedelta(hours=1)

storage_volume = []

for t in solver.run(starttime, endtime, timestep):
    print(t)
    storage_volume.append((W1.volume, W2.volume))
~~~~~~~~~~~~~~~~~~~~


## Structuring your code

Of course, writing the sections from above just down in the typical "script" style
of programs, gets totally confusing when your program grows and needs more then 
50-100 lines. Therefore, it is good practise to seperate your code into functions
with a well defined scope. For an experienced programmer, that comes naturally,
if you are new to programming, please read about structured programming. A simple
structure using functions could look roughly like this:

~~~~~~~~~~~~~.py
import cmf

def create_project():
    """
    Contains parts of the "build setup" section
    """
    p = cmf.project()
    ws1 = p.NewStorage('W1')
    
    return p, ws1
    
def add_boundaries(p):
    ...
    return b_in, b_out
    
def run(p, start, end):
    ...

def plot_result(data):
    ...
~~~~~~~~~~~~~

## CMF and SPOTPY

[`spotpy`](https://github.com/thouska/spotpy) by Tobias Houska is an 
ideal partner to calibrate cmf models. In fact, there is a 
cmf example in spotpy (
[setup](https://github.com/thouska/spotpy/blob/master/spotpy/examples/spot_setup_cmf_lumped.py),
[tutorial](https://github.com/thouska/spotpy/blob/master/spotpy/examples/tutorial_cmf_lumped.py),
[gui](https://github.com/thouska/spotpy/blob/master/spotpy/examples/gui_cmf_lumped.py)
)

You can see there, how
- to write a model class
- you define parameters to calibrate
- and how to seperate fixed structure (`__init__`), and parameter depending structure
(`set_parameters`)