@page Simple_Infiltration.md

[index..](CmfTutStart.md)

# Using simple infiltration in a lumped model

As lumped models are a quite abstract realisation of a catchment, often
a simplified kinematic wave approach is used to connect storages.
Kinematic waves are sufficient for many different flux connections, but
can lead to problems when trying to model infiltration. One way to model
the infiltrationin CMF is the [Simple
Infiltration](http://fb09-pasig.umwelt.uni-giessen.de/cmf/chrome/site/doxygen/classcmf_1_1upslope_1_1connections_1_1_simple_infiltration.html)
approach. This approach checks if all incoming fluxes are higher or
lower than the saturated conductivity of the area. If so, only as much
water infiltrates as the soil can conduct. If it is lower, all incoming
fluxes infiltrate. To shape the flow curve ffull has a value between 1
(target storage is empty) and 0 (target storage is full). To better
adjust to the real catchment the value of the saturaed conductivity can
be changed. Consider the following example mockup code:

    #!python
    # setup
    p = cmf.project()
    c = p.NewCell(0,0,0,1000)
    c.surfacewater_as_storage()
    c.add_layer(2)
    
    # connect and adjust
    # note that for simple infiltration the receiving node comes first
    cmf.SimpleInfiltration(c.layers[0], c.surfacewater)
    
    # adjust the saturated conductivity
    # Ksat can have a value between 0 and 1
    c.layers[0].soil.Ksat = 0.5

author: florianjehn, version: 4 Wed Feb 22 17:02:41 2017
