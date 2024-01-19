@page cmf_lumped_simple Lumped Model with Spotpy Tutorial

## Downloads

Here all data files needed for the tutorial can be downloaded: -
- [Precipitation](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/prec.txt)
- [Discharge](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/discharge.txt)
- [Temperature](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/max_min_avg_temp.txt)

## Getting started

In a former tutorial we learned how we can use CMF to build a simple
lumped model. There, we also learned that calibration by hand is quite
difficult. As we are not the first persons who discover this, smart
people have devised solutions for this problem. One of those solutions
is [Spotpy](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/). Spotpy
allows the automated calibration of models with many different
algorithms and objective functions. This tutorial here is meant to show
you how to use CMF with Spotpy. For this we will be using classes
(object oriented programming). So if you do not yet know what classes
are, it might be helpful to take a
[look](https://docs.python.org/3/tutorial/classes.html). Though, for our
purposes it is enough when you understand classes are just a fancy way
to organize the structure of you code. Obviously you need to have
[SPOTPY](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/#installation)
and [CMF](http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfInstall)
installed on your computer to make this work. To run the model we will
also need data. For the tutorial semi randomly generated data can be
downloaded here:
[discharge](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/discharge.txt),
[temperature](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/max_min_avg_temp.txt)
and
[precipitation](http://fb09-pasig.umwelt.uni-giessen.de/spotpy/download/cmf_lumped/prec.txt).

## Spotpy Interface

Spotpy makes some assumptions about your model. So to make your model
compatible to Spotpy, you need some additional methods which work as an
interface to Spotpy. To make them easier recycable it can be handy to
shift them to an additional class. You do not neccesary have to fully
grasp what those methods do, if you simply want a quick calibration
done.

~~~~~~~~~~{.py}

class SpotpyInterface:
~~~~~~~~~~


The first method calls your model and makes it run with the parameters
Spotpy provides and returns the results the model calculated.

~~~~~~~~~~{.py}

    def simulation(self,vector):
        """
        SpotPy expects a method simulation. This methods calls setparameters
        and runmodels, so SpotPy is satisfied        
        """                  
        paramdict = dict((pp.name,v) for pp,v in zip(self.params,vector))
        self.setparameters(**paramdict)
        resQ = self.runmodel()
        return np.array(resQ)
~~~~~~~~~~


The second method is needed by Spotpy to generate the data with which
the model is evaluated. In our case this is the measured discharge.

~~~~~~~~~~{.py}

    def evaluation(self):
        end = self.begin + cmf.day * len(self.Q)
        return np.array(self.Q[self.begin:end])
~~~~~~~~~~


The third method calls Spotpy's own method for parameter generation.
This means the values for the parameters we need in our model are
generated here.

~~~~~~~~~~{.py}

    def parameters(self):
        return spotpy.parameter.generate(self.params)
~~~~~~~~~~


The final method of the interface is used to define the objective
function. This means on what criteria the model should be judged. In our
case we are using the Nash-Sutcliffe Efficieny, which focusses on peak
flows.

~~~~~~~~~~{.py}

    def objectivefunction(self,simulation,evaluation): 
        return spotpy.objectivefunctions.nashsutcliffe(evaluation,simulation)
~~~~~~~~~~


## The actual model

The structure of the model is the same as the one we used in the first
lumped tutorial. Therefore it should already be a bit familiar. The main
difference is that the model code is now structured in a class, instead
of writing one line under the next one. For this a class is used. To
make use of the Interface we created, we let our model class inherit the
methods from the SpotpyInterface class.

~~~~~~~~~~{.py}

class CMFLumped(SpotpyInterface):
~~~~~~~~~~


The first method of the model class initializes all things we need for
the model, meaning the project, the cell, the layer, an outlet and out
date.

~~~~~~~~~~{.py}

    def __init__(self):
        # create the project
        self.project = cmf.project()
        # For the cell we do not need to specify the x,y,z data as their will only be one cell. 
        # A cell size of 1000 comes in handy because then our data is in m³ the same as in mm.
        self.cell = self.project.NewCell(0,0,0,1000)
        # Also we need to define how when our model begins and in what timesteps it should act
        self.begin = datetime.datetime(1979,1,1)
        self.step = datetime.timedelta(days = 1)
        
        # Add the soil. The parameter is the depth of the layer. In this case the soil is 2 m deep.
        self.soil = self.cell.add_layer(2.0)

        # Create the outlet were all the water of the catchment will be directed to. 
        self.outlet = self.project.NewOutlet("Out", 10,0,0)
        # Evaporation
        cmf.HargreaveET(self.soil, self.cell.transpiration)
~~~~~~~~~~


This method is also used to define our parameters and their range. For
this we need to import a distribution from spotpy. For our case Uniform
is sufficient.

~~~~~~~~~~{.py}

        param = spotpy.parameter.Uniform
        # define all parameters and their range
        self.params = [param("Q0", 1., 500), 
                       param("beta",0.3, 5.),                        
                       param("ETV1", 0., 200), 
                       param("fEVT0", 0., 1)]
        P, Q, T, Tmin, Tmax = self.loadPETQ()
~~~~~~~~~~


Finally the method the method loads in the forcing data and initializes
the rainfall and the meteo station by calling another method (described
below).

~~~~~~~~~~{.py}

        P, Q, T, Tmin, Tmax = self.loadPETQ()
        self.Q = Q
        self.create_stations(P, Q, T, Tmin, Tmax)
~~~~~~~~~~


The method to load in the weather data works the same way as in the
model without spotpy.

~~~~~~~~~~{.py}

    def loadPETQ(self):
        # Data (plain text files)
        fnQ = 'discharge.txt' # enter the name of your discharge data here in m³/day
        fnT = 'max_min_avg_temp.txt' # enter the name of your max, min and average temperature here in °C
        fnP = 'prec.txt' # enter the name of your precipitation data here in mm/day
        # create a empty time series that starts at the begin
        # and goes by step for all elements in the file
        P = cmf.timeseries(self.begin, self.step)
        P.extend(float(Pstr) for Pstr in open(fnP))        
        Q = cmf.timeseries(self.begin,self.step)
        Q.extend(float(Qstr) for Qstr in open(fnQ))
        # Convert m3/s to mm/day
        Q *= 86400 * 1e3 / (2976.41 * 1e6)
        T = cmf.timeseries(self.begin, self.step)
        Tmin = cmf.timeseries(self.begin, self.step)
        Tmax = cmf.timeseries(self.begin, self.step)
        
        # Go through all lines in the file
        for line in open(fnT):
            columns = line.split('\t')
            if len(columns) == 3:
                Tmax.add(float(columns[0]))                 Tmin.add(float(columns[1]))
                T.add(float(columns[2]))
        return P, Q, T, Tmin, Tmax
~~~~~~~~~~


Same goes for the method to create the stations.

~~~~~~~~~~{.py}

    def create_stations(self, P, Q, T, Tmin, Tmax):
        # Create the rainstation and give it the freshly loaded precipitaton data
        self.project.rainfall_stations.add('Rain',P,(0,0,0))
        # This tells every cell in the model (in our case one) to use the rainfall station
        # that is nearest to it. 
        self.project.use_nearest_rainfall()
        
        # Create the meteo station
        meteo = self.project.meteo_stations.add_station('Meteo',(0,0,0))
        # add all data to the station
        meteo.T = T
        meteo.Tmin = Tmin
        meteo.Tmax = Tmax
        # Tell the cell to use the nearest station
        self.project.use_nearest_meteo()
~~~~~~~~~~


One significant difference to the model without Spotpy is the method to
set the values for the parameters. We need this method as we cannot
simply hardcode the values as we did in the model without Spotpy.
Instead the parameters must be flexibly be filled with the values Spotpy
provides. Therefore, we need to create this method to be able to do
this.

~~~~~~~~~~{.py}

    def setparameters(self, **params):
        # Flux from soil to outlet (interflow)
        cmf.PowerLawConnection(self.cell.layers[0], self.outlet,  
                             params["Q0"], V0=500, exponent=params["beta"])
        # Adjustment of the evapotranspiration
        self.cell.set_uptakestress(cmf.VolumeStress(params["ETV1"],                                                     params["ETV1"]] * params["fEVT0"))
~~~~~~~~~~


Finally, we need a method which lets the model run and creates an array
of the output. Here we also create the CMF solver, which solves the
differential equations.

~~~~~~~~~~{.py}

    def runmodel(self):
        try:
            # create the solver, tell him which project it should solve and what its 
            # maximal error should be
            #print(cmf.describe(self.project))
            solver = cmf.CVodeDense(self.project, 1e-8)
            # create an list to save all discharge data
            discharge = []
            # let the solver run for the given timeperiode (this may take some time)
            for t in solver.run(self.begin, self.begin + cmf.day * len(self.Q), self.step):
                # save the discharge for every timestep
                discharge.append(self.outlet.waterbalance(t))
            return discharge
        except RuntimeError:
            return np.array(self.Q[self.begin:self.end + datetime.timedelta(days=1)])*np.nan
~~~~~~~~~~


## Running the model

Now the model is ready to be used with Spotpy. The handling is rather
straightforward. We simply have to create an instance of our model and
forward it to Spotpy. Spotpy additionally needs to know what output
format we want and what our file should be called. Here we can also
define how often the model should run.

~~~~~~~~~~{.py}

model = CMFLumped()
sampler = spotpy.algorithms.lhs(model, dbformat ="csv", dbname = "model")
runs = 100
sampler.sample(runs)
~~~~~~~~~~


We you know start this model you should get a Nash-Sutcliffe Efficieny
of about ~ 0.3. You can play around with other objective functions and
add additional storages and connections, to test wether you can improve
the model. The output you get should be similar to this:

![](spotpy_output.jpg)

Spotpy tells you what algorithm it is using and how much it as
progressed in calculating all the runs we asked it for. When its done,
Spotpy informs us about the beste run (In this case NS = 0.57) and what
parameter set was used for this best run (In our case
[64.4,..., 3.44, 0.66](74.41,). The order in this parameter set is the
one we have defined in the model.

## Old tutorial

In case someone is missing the old tutorial, it can still be found here:
<http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/Cmflumped>


