@page fluxogram_and_get_fluxes How to get a nice visual representation of your model

## Downloads

- [Fluxogram](http://fb09-pasig.umwelt.uni-giessen.de:8081/datafiles/user/florian/Tutorials/fluxogram.py)
- [Get storages and fluxes](http://fb09-pasig.umwelt.uni-giessen.de:8081/datafiles/user/florian/Tutorials/get_storages_fluxes.py)

## Introduction

As your lumped model gets more and more complex it sometimes is
difficult to not lose the overview in your own model. This tutorial will
show you how to find out where the water in your model is stored and
what its flowpaths are. Additionally you will learn how to make a nice
graphic (a fluxogram) out of the data you get. One example for a bit
more complex fluxogram can be seen [here](https://youtu.be/cP0PfDpfW88).


## Building a simple and lumped CMF model

The first step we need to tackle is to construct a small CMF Model which
we will explore in later steps. By now you should be familiar with
simple setups like this. To be able to work your model file and the
files for the fluxogram and get_storages_fluxes all need to be in the
same directory.

~~~~~~~~~~~{.py}
# first import a few handy tools 
import numpy as np
import cmf
import datetime

# also import the fluxogram and our function to find the fluxes and storages
import fluxogram 
import get_storages_fluxes as gsf

# create the project and cell for the lumped model
p = cmf.project()
c = p.NewCell(0,0,0,1000)

# create the storages and the outlet they flow to
c.surfacewater_as_storage()
c.add_layer(5.0)
outlet = p.NewOutlet("outlet",10,0,0)

# add the connections between the storages
cmf.kinematic_wave(c.surfacewater, outlet, 1)
cmf.kinematic_wave(c.surfacewater, c.layers[0], 1)
cmf.kinematic_wave(c.layers[0], outlet, 3)    

# define the timeperiod of the model
begin = datetime.datetime(1979,1,1)
end = begin + 15 * datetime.timedelta(days = 1)
step = datetime.timedelta(days=1)

# create artificial rain data
Prec = cmf.timeseries(begin, step)
Prec.extend(prec for prec in np.random.randint(0, high = 30, size = 15))

# add a rainstation and tell the model to use it
rainstation = p.rainfall_stations.add("Rain", Prec, (0,0,0))
p.use_nearest_rainfall()
~~~~~~~~~~~~~~~

## Creating a solver and letting the model run, while collecting the data

As we now have a model to explore, we can make it run by using a solver
that calculates our fluxes and storages. While the model is running we
also collect the fluxes and storages for every single day seperately.
For this we use get_storages_fluxes.py. In this file are functions
that go recursively through all nodes of the model and collect the data.
For simple usage it is not neccesary to understand exactly how it works,
but it sure does not hurt to try. If you cannot grasp it, do not be
dissapointed. You can practice the leap of faith.

~~~~~~~~~~~{.py}

# create a solver
solver = cmf.CVodeIntegrator(p, 1e-8)

# create empty lists to store the results of the fluxes and storages later
timeseries_fluxes = [[]|timeseries_storages = []]

# let the solver run for our timeperiod in one day steps
for t in solver.run(begin, end, cmf.day):
    # the fluxes are at first returned in a very detailed way
    # as this can be a bit overwhelming we convert them to a version
    # that is easier to use
    fluxes_raw = gsf.flux_of_all_nodes_of_cell(c.rain_source, c, t)
    # the following function is used for this conversion
    fluxes_nicer = gsf.convert_fluxes_for_fluxogram(fluxes_raw)
    # add the fluxes and storages to our timeseries
    timeseries_fluxes.append(fluxes_nicer)
    timeseries_storages.append(gsf.storages_of_cell(c.rain_source, c))
~~~~~~~~~~~

## Clean and display the data

After we now have collected data for the fluxes and storages on every
day our model run we can display it. But before this we have to clean it
of artifacts.

~~~~~~~~~~~{.py}
# due to the way CMF constructs models and how this program searches 
# through it, nodes with no fluxes and storage can occur.
# To get rid of them the following function can be useful
# but be careful as sometimes those empty timeseries emerge because
# you did something wrong and not the code          
timeseries_fluxes_stripped = gsf.del_empty_nodes(timeseries_fluxes)
timeseries_storages_stripped = gsf.del_empty_nodes(timeseries_storages)

# and now lets look on the results
for day, day_counter in zip(timeseries_storages_stripped, range(len(
                                        timeseries_storages_stripped))):
    print("Storages volumes on day: " + str(day_counter))       
    print(day)
    print("")
    
for day, day_counter in zip(timeseries_fluxes_stripped, range(len(
                                            timeseries_fluxes_stripped))):
    print("Fluxes volumes on day: " + str(day_counter))       
    print(day)
    print("")
~~~~~~~~~~~

## Getting ready to make a fluxogram

The fluxogram is animated from a bunch of single pictures that get
stitched together. For this stitching we need ffmpeg. ffmpeg is a handy
project to handle video and audio material. It can be downloaded ​here.
To be able to use it correctly ffmpeg also needs to be added to your
systems PATH. PATH is a variable your computer searches through if you
try to call programs from the console. An explanation how to do this can
be found ​here. You can test if you have done it right if you open a
console and type "ffmpeg". If it does not throw an error and shows you
the options of ffmpeg you have done everything right. If it does not
work directly try restarting your computer.

## Building a fluxogram

After we now have the data from our model and the additional programs we
need, we can make a fluxogram.

~~~~~~~~~~~{.py}
# first create an instance of the fluxogram object
# To work properly the fluxogram needs to know a few things from you
# The first number is the maximal flow that will occur in your model. 
# You do not need the exact number, just a rough estimate
# This holds also true for the second number which is your maximal
# storage volume. The grid_size defines how big your fluxogram will be
# and root = True is needed if your data differs wildly in size. 
# In this case root = True will smooth things out. 
fl = fluxogram.Fluxogram(30, 50, grid_size = 10, root = True)

# After we now have a fluxogram instance to work with we can add the storages
# and fluxes. But as the order must be the same as the order of our data.
# Therefore, we first have to find out what the order of our data is
# and shape our fluxogram accordingly. The following lines can be commented
# out, once the order of the data is known
print(sorted(timeseries_fluxes_stripped[[0].keys()))|print(sorted(timeseries_storages_stripped[0]].keys()))
~~~~~~~~~~~~~
Now we know our model is sorted. In this case the fluxes are sorted
like:

1) Layer0 to outlet 
2) rainfall to surfacewater 
3) surfacewater to layer0 
4) surfacewater to outlet

while the storages are sorted like:

1) Layer0 
2) surfacewater

With this information we can build our fluxogram. But first a little
note on what the structure of a fluxogram is. In a fluxogram every
storage has an order and an offset. In [this](https://postimg.org/image/3p6n3aar9/) 
image you can see how
the fluxogram is sorted using order and offset. So when you create a
storage in a fluxogram you have to have an idea on how your fluxogram
should look and set order and offset accordingly.

~~~~~~~~~~~{.py}
# to add storages we have to give them some input as well
# the sequence is (name_of_storage, starting volume, order, offset)
fl.add_storage("Layer0", 50, 2, 0)
fl.add_storage("Surfacewater", 50, 1, 0)

# Those were the storages we have a volume for. But our water
# is also coming from the rain source and goint to the outlet
# Therefore, we have to add those things as storages as well
fl.add_storage("Rain", 0, 0, 0)
fl.add_storage("Outlet", 0, 2, 1)

# The next step is to add the fluxes between the storages we have
# just created. A flux needs to know what its name is, where it is
# coming from, where it is going to and what its starting volume ist
fl.add_flux("lay0_out", fl.storages[[0],|fl.storages[3]], 30)
fl.add_flux("rain_surf", fl.storages[[2],|fl.storages[1]], 30)
fl.add_flux("surf_lay0", fl.storages[[1],|fl.storages[0]], 30)
fl.add_flux("surf_out", fl.storages[[1],|fl.storages[3]], 30)

# now our fluxogram is complete and we can display it
fl.draw()
fl.show()
~~~~~~~~~~~~~~~~~

## Animating a fluxogram

After we now have a complete fluxogram and all the data we need the
animation is easy. Their is just one thing you should know: The
animation function uses the windows console. If you use Linux or Mac you
have to edit the fluxogram animation method yourselve.

~~~~~~~~~~~{.py}
fl.animate(timeseries_fluxes_stripped, timeseries_storages_stripped, "name_video")
~~~~~~~~~~~

Now you have to wait until the fluxogram animation is ready. During the
process a lot of pictures of the model will be created in your working
directory. But do not worry, the fluxogram will delete all of them once
it has finished the animation. Your result should look something like
[this](https://youtu.be/URAnH_ILzJk). Have fun


