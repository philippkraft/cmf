@page CmfTutWaterbalance

# Waterbalance Connections in CMF

A waterbalance connection can be used to route water through a storage,
while keeping the volume of the water in the storage constant. For
example, this is used in the surfacewater of the cell. Every cell has a
surface water storage, which is mainly used to route water. As it should
not contain any water, except explicitly stated otherwise, the surface
water is connected to the uppermost layer of the cell and all incoming
fluxes are routed to the uppermost layer by a waterbalance connection.

Import all tools.

~~~~~~~~~~~~~{.py}

import cmf
import matplotlib.pyplot as plt
import datetime
~~~~~~~~~~~~~

Create storages.

~~~~~~~~~~~~~{.py}

p = cmf.project()
s1 = p.NewStorage("s1", 0,0,0)
s2 = p.NewStorage("s2", 0,0,10)
s3 = p.NewStorage("s3", 0,0,20)
s4 = p.NewStorage("s4", 0,0,30)
s5 = p.NewStorage("s5", 0,0,40)
~~~~~~~~~~~~~

Create lists to store the data.

~~~~~~~~~~~~~{.py}

flux_s1_to_s3 = [[]|flux_s2_to_s3 = []]
flux_s3_to_s4 = [[]|flux_s3_to_s5 = []]
~~~~~~~~~~~~~

Install the fluxes. Here water is routed from s1 and s2 to s3 (both
technical fluxes) and from s3 to s4 (technical flux) and s5
(waterbalance connection). The technical fluxes all have different
intesities to make the easier to differentiate.

~~~~~~~~~~~~~{.py}

cmf.TechnicalFlux(s1, s3, 10)
cmf.TechnicalFlux(s2, s3, 5)
cmf.waterbalance_connection(s3, s4)
cmf.TechnicalFlux(s3, s5, 2.5)
~~~~~~~~~~~~~

Run the model and store all fluxes.

~~~~~~~~~~~~~{.py}

solver = cmf.CVodeIntegrator(p)

start = datetime.datetime(2017, 12, 1)
end = datetime.datetime(2017, 12, 15)

for t in solver.run(start, end, cmf.day):
    flux_s1_to_s3.append(s1.flux_to(s3, t))
    flux_s2_to_s3.append(s2.flux_to(s3, t))
    flux_s3_to_s4.append(s3.flux_to(s4, t))
    flux_s3_to_s5.append(s3.flux_to(s5, t))
~~~~~~~~~~~~~

Plot the fluxes.

~~~~~~~~~~~~~{.py}

plt.plot(flux_s1_to_s3, label="Technical flux 10 m³")
plt.plot(flux_s2_to_s3, label="Technical flux 5 m³")   
plt.plot(flux_s3_to_s4, label="Waterbalance connection")    
plt.plot(flux_s3_to_s5, label="Technical flux 2.5 m³")     
 
plt.ylabel("Flux [[m³/day]")|plt.xlabel("Time [days]]")
plt.legend()
~~~~~~~~~~~~~

The resulting image should look something like this:

![](waterbalance_connection.png​)

Here we can see how the different connections behave. Storage 3 gets
water from storage 1 and 2 and looses water to storage 4 and 5. The
connection from storage 3 to storage 4 is the waterbalance connection.
The shape of this connections is shaped by the combination of the other
connections. At start the waterbalance connection routes the most water,
as it transports all incoming water (from storage 1 and storage 2) minus
the outgoing water (to storage 5). After storage 2 runs dry, the
waterbalance connection transports less water then the technical flux
from storage 1 to storage 3, as now the waterbalance is: flux from
storage 1 to storage 3 minus flux from storage 3 to storage 5. When
storage 1 runs dry, the waterbalance connection does not transport water
any more, as now all remaining water in storage 3 is routed to storage
5.

author: florianjehn, version: 5 Thu Dec 21 13:42:58 2017
