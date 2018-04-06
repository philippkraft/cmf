@page Cmflumped

[index..](@ref tutorial)

# Lumped Model with Spotpy Tutorial

## Downloads

Here all files needed for the tutorial can be downloaded. Additionally
they can be found at the relevant sections: -
[Precipitation](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/prec.txt)
-
[Discharge](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/discharge.txt)
-
[Temperature](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/max_min_avg_temp.txt)
- [Model Template
(empty)](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/tutorial_template.py)
- [Model Template
(filled)](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/tutorial_model.py)
- [Datawriter
Function](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/datawriter.py)
- [Model
Structure](http://s33.postimg.org/ioqfyrlfz/most_complex_fulda_v2_englisch.jpg)
- [Dotty
Plotter](http://www.file-upload.net/download-11587863/dotty_plot.py.html)
\<--- noch ins Uni Netz

## Getting started

This tutorial has the goal to teach you to set up, run, calibrate and
validate your own lumped hydrological model (and make a few nice figures
for showing off). For calibration and validation
[SPOTPY](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/) will be used.
The SPOTPY code will only be explained in the detail needed for a simple
calibration and validation. For additional information see [SPOTPY
Tutorial](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/Tutorial/1-Introduction/).
It is useful but not necessary to have worked through the [basic CMF
tutorials](http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/tutorial#Gettingstarted).
The lumped model covered in this tutorial is relatively complex. It
might seem a bit overwhelming at first, but this way all the most common
parts of CMF for lumped model building are covered. That way you will be
able to build your own model after the tutorial more easily. Obviously
you need to have
[SPOTPY](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/#installation)
and [CMF](http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfInstall)
installed on your computer to make this work. The model structure we are
going to implement is shown
[here](http://s33.postimg.org/ioqfyrlfz/most_complex_fulda_v2_englisch.jpg).
To run the model we will also need data. For the tutorial semi randomly
generated data can be downloaded here:
[discharge](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/discharge.txt),
[temperature](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/max_min_avg_temp.txt)
and
[precipitation](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/prec.txt).

## Introducing the template

For the further steps we will use a template, that can be downloaded
[here](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/tutorial_template.py).
With this template you can build your own model more easily. In this
section you will get a rough introduction to the different parts of the
template. Additionally you will need the [datawriter
function](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/datawriter.py).

''Defining model timespan, name and input data files''

    #!python
    # Calibration time span
    begin = 1980 # enter the starting year of the calibration here
    # do not include the first year or so of your data to allow the model a
    # spin up period before the calibration
    end = 1985 # enter the last year of the calibration here
    # the remaining years of your data set will be used for validation
    
    prefix = 'mymodel'# enter model name here 
    
    # Number of runs
    runs = 10 # enter the number of runs here 
    # for example:
    # - use ten runs to test if your model works at all
    # - use about 50k to 150k runs to do the real calibration
    
    # Data (plain text files)
    fnQ = 'discharge.txt' # enter the name of your discharge data here in m³/day
    fnT = 'max_min_avg_temp.txt' # enter the name of your max, min and average temperature here in °C
    fnP = 'prec.txt' # enter the name of your precipitation data here in mm/day

Here we define the periods of the data that will be used for calibration
and validation, give our model a name, define how often it will run and
what the names of our input files are.

''Fixed parts setup''

~~~~~~~~~~~~~ 
#!python
class lumped_model(object):
    """
    Contains the whole model
    """
    def __init__(self,begin,end):
        """
        Initializes the model and builds the core setup  
        """     
        # copy model parameters here
        self.params = [param('name',min_value,max_value)] # enter your own values here directly

        # only use one core (quicker for small models)
        cmf.set_parallel_threads(1)
        
        # Regen
        self.makestations(P,T,Tmin,Tmax)
        self.project = p
        self.begin = begin
        self.end = end   
~~~~~~~~~~~~~

In the __init__ method we define all our parameters of the model and
their possible values, set up the main structure of the model and read
in our data.

''Flexible parts setup:''

    #!python
    def setparameters(self,name,   ):  # this list has to be identical with the one above
        """
        sets the parameters, all parameterized connections will be created anew in every run    
        """
        # enter changeable model connections and nodes here

In this part our flexible model structures will be defined, meaning the
ones where the parameter values are changed each calibration run.
Further explanations will be given in the 'Building the model' segment.

The following methods in the template loadPETQ and makestations are used
to read in the weather files and to bring the data into a format that
can be understood by CMF.

The next method in template is runmodel. We can also leave it as it is,
with one line as an exception:

    #!python
        solver = cmf.CVodeIntegrator(self.project,1e-8)

In this line we can define which numerical solver CMF will use and what
its step will be. In this case we use the CVode integrator because it
compensates for numerical problems in the snow routine which we will use
later. If you don't need the snow routine in your models you can also
use an simpler solver like Explicit Euler to speed things up. For
further information on the numerical solvers in CMF look
[here](http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfTutSolver),
if you are unsure which one to use [this
paper](http://onlinelibrary.wiley.com/doi/10.1002/hyp.7899/full) might
help you.

All other methods are for spotpy or visualisation and need not be
changed during the tutorial.

## Adding the data

The template accepts plain .txt files as input data in the same format
as the files that can be downloaded above. To read them in the method
loadPETQ is used. To do so you have to add the following lines to your
init method:

    #!python
            P,T,Tmin,Tmax,Q = self.loadPETQ()
            self.Q=Q

## Building the model

'''Fixed parts'''

The first thing we have to do, is to indentify all our fixed model
parts, meaning all parts that will be the same for every model run. In
our case those are: - The CMF project as a whole - Our lumped cell for
the whole catchment - The snow routine - The surface water storage - The
soil storage - The two groundwater storages - The evapotranspiration -
Our river - The drinking water outlet - The interception storage As you
can see the fixed parts are all storages or storage like entities.
Therefore, all flexible parts are the connections between all those
storages. This is due to the structure of lumped models as the storages
are mainly defined about the in- and outfluxes. To implement those fixed
structures in CMF is relatively straightforward. The implementation of
the fixed model structures takes place in the init method. The first
thing we have to do is to make a CMF project, the structure which
contains all model parts.

    #!python
            self.project = cmf.project()
            p = self.project

All following structures refer to this project. The next thing we need
to add is a cell. In CMF cells are the units of the [horizontal
discretization](http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfTutCell).
As we are making a lumped model, the cell doesn't need a certain
position in space, so we can just set the coordinates x,y and z all to
zero. Cells also need a size, but as we are constructing a lumped model,
this also doesn't really apply. So we can just set our size to 1000, so
our output will have the unit mm '''(Phillip nochmal fragen, ob ich das
richtig verstanden habe)'''.

    #!python
            c = p.NewCell(0,0,0,1000)

As the next structures we will add are the snow routine and the surface
water storage. The snow routine is holding back water if the temperature
is below a certain threshold temperature (snow melt rate). The surface
water storage is needed for a more realistic routing.

    #!python
            c.add_storage('Snow','S')
            cmf.Snowfall(c.snow,c)
            
            c.surfacewater_as_storage()

The soil and groundwater storages are added as layers to our cell. The
argument defines the depth of the layer. After adding them we fill them
with initial amounts of water, so our model has a quicker warm-up phase.
As we have added a soil layer, we can also add the evapotranspiration
from the soil. The evapotranspiration can be calculated in different
ways in CMF (more information about CMF's handling of evapotranspiration
can be found
[here](http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfTutET)). Here
the
[Hargreaves](http://www.zohrabsamani.com/research_material/files/Hargreaves-samani.pdf)
approach is choosen as it needs only little input data. If more data,
e.g. wind velocity, is available more sophisticated approaches (like
[Penman-Monteith](http://www.fao.org/docrep/x0490e/x0490e06.htm)) can be
used as well.

    #!python
            soil = c.add_layer(2.0)
            gw_upper = c.add_layer(5.0) 
            gw_lower = c.add_layer(20.0)
            
            # Fill storages
            c.layers[0].volume = 100
            c.layers[1].volume = 200
            c.layers[2].volume = 300
    
            cmf.HargreaveET(soil,c.transpiration)

As a river is also a large storage for water it is added to this model.
But as our model is a lumped one, we don't need coordinates. The area is
referring to the surface of the river. This value must be calculated for
every catchment seperately. Additionally you have to add a depth of the
river, so CMF can calculate the volume of the river.

    #!python
            self.river = p.NewOpenStorage(name="River",x=0,y=0,z=0, area = 100)
            # Giving the River a mean depth
            self.river.potential = 1.5 

The last thing we will add is an representation of a drinking water
pump, the outlet to the catchment and an interception storage.

    #!python
            # add the drinking water outlet
            self.drinking_water = p.NewOutlet('drinking_water',20,0,0)
       
            # Outlet
            self.outlet = p.NewOutlet('outlet',10,0,0)
            
            # Storage for the interception
            I=c.add_storage('Canopy','C')

Youre init method should now look something like
[this](http://www.file-upload.net/download-11542855/init_ohne_param.py.html)

'''Flexible parts'''

As we now have added the fixed main structure of our model we have to
connect them. This will be done in the method setparameters. To be able
to work with the structures we have already implemented, we have to
adopt them. This is done like the following:

    #!python
            p = self.project
            c = p[0]
            outlet = self.outlet
            river = self.river
            drinking_water = self.drinking_water

The connection we will use the most is the [kinematic
wave](http://fb09-pasig.umwelt.uni-giessen.de/cmf/chrome/site/doxygen/classcmf_1_1water_1_1kinematic__wave.html).
This is because with the kinemativ wave many processes can be
represented well enough for a lumped model. The syntax for the kinematic
wave is:

    #!python
            cmf.kinematic_wave(source, target, residence time, exponent, V0)

Where V0 represents '''???'''. To model the flux of a drinking water
system a constant flux is needed. This is provide by the [technical
flux](http://fb09-pasig.umwelt.uni-giessen.de/cmf/chrome/site/doxygen/classcmf_1_1water_1_1_technical_flux.html#a22a23463bd8a7a7d1fa2fe9ff87aef53)
in CMF:

    #!python
            cmf.TechnicalFlux(source,target,max_flow,minmal_flow,cmf.day)

'''Was macht cmf.day?'''. The third connection we will use is the
[waterbalance
connection](http://fb09-pasig.umwelt.uni-giessen.de/cmf/chrome/site/doxygen/classcmf_1_1water_1_1waterbalance__connection.html).
It is used to '''???''' and has the following syntax:

    #!python
            cmf.waterbalance_connection(source, target)

Try to add those connections between the storages as you think it would
best resemble our model structure. At the end of this segment a file
will be provided how it should look like.

As it is sometimes neccesarry to make the snow melt temperature and the
snow melt rate changeable parameters, we also implement it here and not
in the fixed section. This is done like the following:

    #!python
            snowmelt_surf = cmf.SimpleTindexSnowMelt(c.snow,target,cell,rate=meltrate)
            # set snowmelt temperature
            cmf.Weather.snow_threshold = snow_melt_temp

The last thing we will add is the interception to simulate water hold
back by vegetation (more Information about Interception in CMF can be
found
[here](http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfTutIntercept):

    #!python
            # Splits the rainfall in interzeption and throughfall
            cmf.Rainfall(c.canopy,c, False, True)
            cmf.Rainfall(c.surfacewater,c, True, False)
            # Makes a overflow for the interception storage
            cmf.RutterInterception(c.canopy,c.surfacewater,c)
            # Transpiration on the plants is added
            cmf.CanopyStorageEvaporation(c.canopy,c.evaporation,c)
            # Sets the parameters for the interception       
            c.vegetation.LAI= LAI

Your setparameters method should now look something like
[this](http://www.file-upload.net/download-11542922/setparameters_ohne_param.py.html).

## Calibrating the model

You now have a model almost ready for calibration. The last thing we
have to do is to tell our model what the parameters are we want to
calibrate and in what range we want them to be calibrated. For this we
have to change two parts of our model we have created so far. The first
one is the parameter list of the setparameter method. Here we have to
fill in all parameters we have defined in the setparameters method. So
for the example file for the setparameters method it should look like
this:

    #!python
        def setparameters(self,tr_soil,tr_surf,tr_GW_l, tr_GW_u, tr_fulda,
                          V0_soil,  
                          V0_surf,
                          beta_P, beta_surf_to_out, beta_surf_to_soil,
                          ETV1,
                          fETV0,
                          meltrate,
                          snow_melt_temp,
                          Qd_max,
                          LAI
                          ):

Here I have sorted the parameters to get an better overview, but you
don't have to do that. To define the range of our calibration for the
parameters we have to switch to the init method again. Here we have to
assign the parameteres to our model class. To add your parameters there
you have to give them a name and define a lower and upper boundary for
them. For example: If I want to add two parameters with the names
"residence_time" and "runoff_beta" with the values 1 to 12
respectiveley 0.1 to 2.0 the code would look something like this:

    #!python
            self.params = [param('residence_time',1,12),('runoff_beta',0.1,2.0)]

Following this example the params list for our example would look like
this:

~~~~~~~~~~~~~ 
#!python
               # tr_S = Residence time of the water in the soil
self.params = [param('tr_soil',1.,1000.),
               # tr_GW_u = Residence time in the upper groundwater
               param('tr_GW_u',1.,2000.),
               # tr_GW_l = residence time in the lower groundwater
               param('tr_GW_l',1.,2000.),
                # tr_surf_to_out = Residence time from surface 
               param('tr_surf',0.01,30),
               # ETV1 = the Volume that defines that the evaporation is lowered because of not enough water
               param('ETV1',0.,100.),
               # fETV0 = factor the ET is multiplied with when water is low
               param('fETV0',0.,1.),
                # beta = Exponent that changes the form of the flux from the soil
               param('beta_P',0.5,7.0),
               # beta_surf_to_out und beta_surf_to_soil exist to route the water
               # during storm events more realistically.
               param('beta_surf_to_out',2.0,3.0),
               param('beta_surf_to_soil',0.3,0.7),
                # Rate of snow melt
               param('meltrate',0.1,14.),
               # V0_soil = field capacity for the soil
               param('V0_soil',10.,250.),
               # V0_surf = "field capacity" like feature of the surfaces
               param('V0_surf',1.,50.),
               #  Snow_melt_temp = Temperature at which the snow melts (needed because of averaged temp
               param('snow_melt_temp',0.0,7.0) ,
               # tr_fulda = Residence time in the river (in days)
               param('tr_fulda', 3.5, 25),       
               #Qd_max = maximal flux from lower groundwater to drinking water production
               param('Qd_max', 2.,30.),
               # LAI = leaf area index
               param('LAI', 2.,7.)
               ]    
~~~~~~~~~~~~~

Again I have sorted and commented the parameters for a better overview.
I recommend to do the same in your own model so you don't get lost in
them.

Now the model is ready to be calibrated. But before we start we have to
make one decision. What kind of calibration algorithm do we want to use?
[Spotpy](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/) includes a
wide range of algorithms. For starters I would recommend [latin
hypercube
sampling](https://en.wikipedia.org/wiki/Latin_hypercube_sampling) as it
is method that produces good results quickly, but the theory behind it
is still easy to understand. The template is pre configured to a simple
Monte Carlo algorithm, so if we want to use latin hypercube sampling we
have to change one line a bit. You can find the following line in the
main method:

    #!python
        from spotpy.algorithms import mc as Sampler

To use latin hpyercube sampling (or any other sampling provided by
Spotpy you simple have to change the imported samper. For our example we
have to change "mc" to "lhs". '''Tobi fragen ob er noch schlaue Sachen
hier hinschreiben kann'''.

And now we can calibrate our model\! If you have done everything right
the model should be complete now and you can simply start calibrating by
running your code. If you didn't change the amount of runs, it should
run the model ten times and then tell you what the best run was (this
should require 1 or 2 minutes, depending on your hardware). If you did
something wrong and can't figure it out our you simply didn't want to do
all the steps above
[here](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/tutorial_model.py)
is a working piece of code for our example.

After you have run the model calibration CMF will have produced a few
files. A simulation.csv and a parameters.csv. The parameter.csv contains
all the parameter values of all runs together with the resulting value
of the objective function (here the Nash-Sutcliffe efficiency), while
simulation.csv only contains those runs with a Nash-Sutcliffe higher
than the threshold (the minimal value of the objective function the
model should reach), together with the simulated discharge for all days.
By viewing those files you can find out which parameter combination gave
you the best results and what your best Nash-Sutcliffe was. Also they
are the foundation for the plots we will produce in the next section.

## Visualization of the results

If you have used the code for the whole model supplemented above you
might have noticed that a .png appeared in the directoy of your model.
If you didn't use the template you have to add one line at the end of
you model code to activate the method plotsimulation:

    #!python
        model.plotsimulation(threshold)

The treshold determines above which Nash-Sutcliffe model runs are used
for the plot. The plot iself should look something like
[this](http://postimg.org/image/oh0zfrgox/). The black line is the
observed discharge, the red line is the simulated discharge of the best
run and the yellow outlining represents the uncertainty.

With the Dotty Plot function (found in Downloads) and the
\*-parameter.csv you now can plot you own dotty plots. Just fill in the
name of your parameter file in the function. Dotty plots are used for
sensitivity analysis in order to identify which parameters are more
identifiable than others and to establish more feasible ranges for the
parameters. In a dotty plot the model effiency is plotted against the
parameter value, showing which parameter value led to a good model run.
Examples can be seen
[here](http://s32.postimg.org/skvz8gt1g/snow_melt_temp_dotty.jpg) and
[here](http://s32.postimg.org/hyxv7v1ms/tr_fulda_dotty.jpg). The first
plot shows an parameter that doesn't effect the model very much. No
matter how high or low the snow melt temperatur is, the model stays
pretty much the same. It is different for the second picture. Here you
see a clear trend. The smaller the value of the parameter, the better
the model run. This information can be used to give your parameter
different boundaries. For example the parameter of the second picture
should get a lower minimum as the trend shows in that direction.

## Validating the model

After we now have at least a few runs of with good parameters we can
start the validation of our model. For this we will need our results and
make a few changes in our model.

To do here:

- explain how the validation works in CMF

- explain how to use the plot functions to get a nice visualization of
the validation

author: florianjehn, version: 85 Tue Nov 15 12:48:32 2016
