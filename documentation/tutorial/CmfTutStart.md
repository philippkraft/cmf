@page CmfTutStart

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

  - @subpage CmfInstall "Installation"
  - @subpage InstallCheck "Check if cmf is correctly installed"
  - @subpage CmfTutProject "Creating projects"

### A first simple model

  - @subpage CmfTutFirstModel "Let it flow... the first simple model"
  - @subpage CmfTutBoundary "Boundary conditions"
  - @subpage CmfTutFluxes "Query fluxes and states"
  - @subpage CmfTutSolver "Choosing a solver for the ODE system"

### Spatial context

  - @subpage CmfTutUnits "Units in cmf"
  - @subpage CmfTutSpaceTime "Time and space in cmf"
  - @subpage CmfTutCell "cells and reaches"

### Conceptual Connections

  - @subpage CmfTutKinematicWave "Kinematic waves" as swiss army knife 
    for conceptual model building
  - @subpage Simple_Infiltration "Infiltration"
  - @subpage CmfTutTechnical "Technical Flux"
  - @subpage CmfTutWaterbalance "Waterbalance Connection"

### Introducing hydraulic head in cmf

  - @subpage CmfTutRetentioncurve "Hydraulic head in the subsurface"
  - @subpage CmfTutVolumeHeight "Hydraulic head in surface water"
  - @subpage CmfTutBoundary2 "Hydraulic head based boundary conditions"

## Models

In the next models, the basics from above are put together. The models
use different techniques for visualization also.

### Soil column (1D)

  - @subpage CmfTut1d "Simple 1D Model"
  - @subpage CmfTutSoluteTransport1D "Solute transport"

### Distribution of rainfall and other meteorological data

  - @subpage CmfTutTestData "Test data"
  - @subpage CmfTutMeteostation "Using stations"

### Interaction with the atmosphere

  - @subpage CmfTutET "Evapotranspiration"
  - @subpage CmfTutIntercept "Interception"
  - @subpage CmfTutSnow "Snow"

### Hillslope (2D)

  - @subpage CmfTutDarcianLateralFlow "Darcian flow connections"
  - @subpage CmfTutSurfaceRunoff "Surface runoff"
  - @subpage CmfTut2d "Simple hillslope model"

### Channeled flow

  - @subpage CmfTutChannel "Simple channel flow model"

### 3D Model

  - @subpage CmfTut3d "3D Model" (this tutorial is a
    [stub](WikiPedia:Wikipedia:stub))

### Lumped Model

  - @subpage CMF_lumped_without_spotpy "Lumped Model without Spotpy"
  - @subpage Cmf_lumped_simple "Lumped Model with Spotpy"

### Semi-distributed Model

  - @subpage semi_distributed "Semi-distributed Model"

### Distributed Model

  - Distributed Model

### Advanced usages

  - @subpage Descriptor "Overview representation of the storages and fluxes in a lumped model"
  - Dual and multi-permeability models
  - @subpage CmfTutObjectiveFunction "What objective function should I use?"
    (this tutorial is a [stub](WikiPedia:Wikipedia:stub))

author: florianjehn, version: 58 Fri Jan 12 14:02:40 2018
