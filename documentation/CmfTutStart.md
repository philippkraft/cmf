@page CmfTutStart.md

# Tutorial

### What you need to know for using cmf

First, you need to have some knowledge of Python - how to write loops,
functions, perform math etc. Python is a programming language gaining
influence for scientific computing since years, even Nature wrote aboute
it
[(Perkel 2015)](http://www.nature.com/news/programming-pick-up-python-1.16833).
You should know how to read and write data from file. If you have
already some programming skills, but you are new to Python, please read
first the official [Python
tutorial](http://docs.python.org/3/tutorial/), especially section 1-10.
A second good source is the freely available book [Python in
Hydrology](http://www.greenteapress.com/pythonhydro/pythonhydro.html),
as a compagnion to the official tutorial.

To see what happens some knowledge of [NumPy](http://numpy.org) and
[MatPlotLib](http://matplotlib.org) is very helpful, to get a visual
output of your results.

If you are completely new to programming, using cmf will be quite hard,
but you don't need to be the super-hacker. If cmf is for you an argument
to learn programming with Python (you will have use for that beyond cmf.
This wiki and its webserver for instance is written in Python) you might
like to have a look at these free books: [Think Python
(3)](http://greenteapress.com/wp/think-python-2e/) or [Learn Python (2)
the hard way](http://learnpythonthehardway.org/book/).

### Python 2 vs. 3

The programming language Python has undergone some changes between the
version 2 and 3. Although Python 3 is available since 2008, it took
quite a while until all major libraries were compatible to Python 3.
Since 2014 this is the case and cmf is available for Python 3 also. I
recommend to use it, since cmf will not be available for Python 2 on
Windows by 2018. However, Python 2 is still around. The tutorials are
made to run in both versions.

### How to deal with the tutorials

The best way to try out this tutorial is from an interactive environment
line, like IPython or Spyder (see CmfRecommendedSoftwareEnvironment).
The tutorials do not include excercises. However, to understand what is
going on, try to understand every line you type or copy (better to type)
and make changes to the code to see what happens. Change parameters,
extend the problem and so on. For the later tutorials it is a good
exercise to combine tutorials, eg. write a 1D model with real wheater
data on your own, rather than download it directly.

## Getting started

  - [Installation](CmfInstall.md)
  - [Check if cmf is correctly installed](InstallCheck.md)
  - [Creating projects](CmfTutProject.md)

### A first simple model

  - [Let it flow... the first simple model](CmfTutFirstModel.md)
  - [Boundary conditions](CmfTutBoundary.md)
  - [Query fluxes and states](CmfTutFluxes.md)
  - [Choosing a solver for the ODE system](CmfTutSolver.md)

### Spatial context

  - [Units in cmf](CmfTutUnits.md)
  - [Time and space in cmf](CmfTutSpaceTime.md)
  - [cells and reaches](CmfTutCell.md)

### Conceptual Connections

  - [Kinematic waves as swiss army knife for conceptual model
    building](CmfTutKinematicWave.md)
  - [Simple Infiltration](Simple_Infiltration.md)
  - [Technical Flux](CmfTutTechnical.md)
  - [Waterbalance Connection](CmfTutWaterbalance.md)

### Introducing hydraulic head in cmf

  - [Hydraulic head in the subsurface](CmfTutRetentioncurve.md)
  - [Hydraulic head in surface water](CmfTutVolumeHeight.md)
  - [Hydraulic head based boundary conditions](CmfTutBoundary2.md)

## Models

In the next models, the basics from above are put together. The models
use different techniques for visualization also.

### Soil column (1D)

  - [Simple 1D Model](CmfTut1d.md)
  - [Solute transport](CmfTutSoluteTransport1D.md)

### Distribution of rainfall and other meteorological data

  - [Test data](CmfTutTestData.md)
  - [Using stations](CmfTutMeteostation.md)

### Interaction with the atmosphere

  - [Evapotranspiration](CmfTutET.md)
  - [Interception](CmfTutIntercept.md)
  - [Snow](CmfTutSnow.md)

### Hillslope (2D)

  - [Darcian flow connections](CmfTutDarcianLateralFlow.md)
  - [Surface runoff](CmfTutSurfaceRunoff.md)
  - [Simple hillslope model](CmfTut2d.md)

### Channeled flow

  - [Simple channel flow model](CmfTutChannel.md)

### 3D Model

  - [3D Model](CmfTut3d.md) (this tutorial is a
    [stub](WikiPedia:Wikipedia:stub))

### Lumped Model

  - [Lumped Model without Spotpy](CMF_lumped_without_spotpy.md)
  - [Lumped Model with Spotpy](Cmf_lumped_simple.md)

### Semi-distributed Model

  - [Semi-distributed Model](semi_distributed.md)

### Distributed Model

  - Distributed Model

### Advanced usages

  - [Overview representation of the storages and fluxes in a lumped
    model](Descriptor.md)
  - [Dual and multi-permeability models](CmfTutMacropore.md)
  - [What objective function should I use?](CmfTutObjectiveFunction.md)
    (this tutorial is a [stub](WikiPedia:Wikipedia:stub))

author: florianjehn, version: 58 Fri Jan 12 14:02:40 2018
