@page CmfTutTechnical



# Technical fluxes in CMF

A technical flux is meant to simulate an artificial flow of water, e.g.
a well extracting a fixed amount of water per day. The usage is quite
simple. One only has to define what storages are to be connected and
what amount of water should be transferred on every timestep.

Load the needed packages.

~~~~~~~~~~~~~{.py}

import cmf
import matplotlib.pyplot as plt
import datetime
~~~~~~~~~~~~~

Create the project and two storages. The first one is filled with 100 m³
of water at start. Save the starting volumes of the storages.

~~~~~~~~~~~~~{.py}

s1 = p.NewStorage("s1", 0,0,0)
s2 = p.NewStorage("s2", 0,0,10)

s1.volume = 100

vol_s1 = [[s1.volume]|vol_s2 = [s2.volume]]
~~~~~~~~~~~~~

Connect the two storages with a technical flux, that transfers 10 m³/day
(maximal, less when no water is present in the storage). The technical
flux has also a min argument (default = 0) which specifies the minimal
amount of water that should remain in the source storage.

~~~~~~~~~~~~~{.py}

max_flux = 10
cmf.TechnicalFlux(s1, s2, max_flux)
~~~~~~~~~~~~~

Prepare the variables to run the model for a few days.

~~~~~~~~~~~~~{.py}

solver = cmf.CVodeIntegrator(p)

start = datetime.datetime(2017, 12, 1)
end = datetime.datetime(2017, 12, 15)
~~~~~~~~~~~~~

Run the model. Save the volumes at every day.

~~~~~~~~~~~~~{.py}

for t in solver.run(start, end, cmf.day):
    vol_s1.append(s1.volume)
    vol_s2.append(s2.volume)
~~~~~~~~~~~~~

Plot the volumes.

~~~~~~~~~~~~~{.py}

plt.plot(vol_s1, label="Volume Storage 1")
plt.plot(vol_s2, label="Volume Storage 2")    
plt.ylabel("Volume [[m³]")|plt.xlabel("Time [days]]")
plt.legend()
~~~~~~~~~~~~~

The resulting image should something like this:

![](technical_flux.png)

We can see that every day 10 m³ are transferred, until the first storage
has no water left to transfer.

@author florianjehn, version: 5 Thu Dec 21 13:02:37 2017
