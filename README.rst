.. image:: https://img.shields.io/pypi/v/cmf.svg
    :target: https://pypi.python.org/pypi/cmf/
.. image:: https://img.shields.io/pypi/pyversions/cmf.svg
    :target: https://pypi.python.org/pypi/cmf#downloads
.. image:: https://img.shields.io/badge/docs-tutorial-yellow.svg
    :target: http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfTutStart
.. image:: https://img.shields.io/travis/philippkraft/cmf/master.svg
    :target: https://travis-ci.org/philippkraft/cmf
.. image:: https://img.shields.io/badge/license-GPL-green.svg
    :target: https://opensource.org/licenses/GPL-2.0
.. image:: https://zenodo.org/badge/DOI/10.5281/zenodo.1125290.svg
   :target: https://doi.org/10.5281/zenodo.1125291

Modelling water and solute fluxes
===================================

cmf is a programming library to create hydrological models, which are highly modular and connectible to other
models developed using a multiple hypotheses background. Although written in C++, its primary usage is to be compiled as an extension to other programming languages, using SWIG. Researchers can build individual models,
targeting their scientific question by using the library objects like water storages, boundary conditions,
fluxes and solvers. cmf uses the finite volume method to set up a wide range of models of water flow through
your study area. Resulting models can range from **lumped conceptual models** to **fully distributed darcian models**
and everything in between.

*cmf* is published as free software under GPLv3. (c) 2007-2017 by Philipp Kraft and the
Institute of Landscape Ecology and Resources Management, Justus-Liebig Universität Gießen.

Documentation
---------------
Can be found here: http://fb09-pasig.umwelt.uni-giessen.de/cmf

Publication
------------

The basic publication of cmf is:
    Kraft, P., Vaché, K.B., Frede, H.-G. Breuer, L. 2011. A hydrological programming language extension for integrated catchment models, Environmental Modelling & Software, `doi: 10.1016/j.envsoft.2010.12.009 <https://doi.org/10.1016/j.envsoft.2010.12.009>`_

Published applications of cmf can be found in the documentation: http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/PublicationList

Scientific Background
==========================

In the last decade, the application of classical hydrological rainfall runoff models is disputed as a valid
method for understanding water transport in landscapes (eg. Beven 2002 and 2006, Seibert and McDonnell 2002,
Sivapalan et al. 2003, Kirchner 2006, Tetzlaff et al. 2008), since current models lack the ability for
formulation and rejection of hypotheses. Vache and McDonnell (2006) developed a framework for the rejection
of model structures, where each model structure is understood as a hypothesis on runoff generation. This work is
a newly developed generalized form of the Vache-McDonnell rejectionist framework, called the
Catchment Modelling Framework (CMF, Kraft et al. 2011 and 2012). It is an extension to the programming
language Python, written in C++, offering a toolkit for the set up of a wide range of hydrological models,
following the `finite volume approach <http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/FiniteVolumeMethod>`_ by Qu and Duffy (2007).
The framework exports objects to set up a network of water and solute fluxes.

Buytaert et al. (2008) and Clark et al. (2011) call also for such modular frameworks as
a tool for the formulation, implementation, test and rejection of process hypotheses. Buytaert et al. (2008)
demand such frameworks to be portable, accessible and modular. While hydrologists debate the theoretical application 
limits of runoff models for solute transport in landscapes, a growing demand of integrated landscape models for the
integration of lateral transport of matter by runoff arises in interdisciplinary projects. With this background,
modular hydrological frameworks need to be designed for simplified data exchange during the model runtime for
interfacing the hydrology with models from different disciplines, like CMF.

CMF is portable, accessible and modular as an open source extension to the Python language and can be used for
the formulation of different hypotheses. The principle of the connection of CMF with models from different
disciplines is shown by Kraft et al. (2010), and Haas et al. (2012) show the relevance of tightly connected
models of transport and turnover for the emission of greenhouse gases from ecosystems.

 - Beven, K., 2006. Searching for the Holy Grail of scientific hydrology. Hydrol.Earth Syst.Sci 10, 609-618.
 - Beven, K.J., 2002. Towards an alternative blueprint for a physically-based digitally simulated hydrologic response modelling system. Hydrol.Proc. 16, 189-206.
 - Buytaert, W., Reusser, D., Krause, S., Renaud, J.P., 2008. Why can't we do better than Topmodel? Hydrol.Proc. 22, 4175-4179.
 - Clark, M.P., Kavetski, D., Fenicia, F., 2011. Pursuing the method of multiple working hypotheses for hydrological modeling. Water Resour.Res 47.
 - Haas, E., Klatt, S., Fröhlich, A., Kraft, P., Werner, C., Kiese, R., Grote, R., Breuer, L., Butterbach-Bahl, K., 2012. LandscapeDNDC: a process model for simulation of biosphere-atmosphere-hydrosphere exchange processes at site and regional scale. Landscape Ecol. DOI 10.1007/s10980-012-9772-x
 - Kirchner, J., 2006. Getting the right answers for the right reasons: Linking measurements, analyses, and models to advance the science of hydrology. Water Resour.Res. 42, W03S04, doi:10.1029/2005WR004362.
 - Kraft, P., Multsch, S., Vache, K. B., Frede, H.-G. and Breuer, L.: Using Python as a coupling platform for integrated catchment models, Adv. Geosci., 27, 51-56, doi:10.5194/adgeo-27-51-2010, 2010.
 - Kraft, P., 2012. A hydrological programming language extension for integrated catchment models, Dissertation, Justus-Liebig-Universität, Gießen, 16 March. [online] Available from: http://geb.uni-giessen.de/geb/volltexte/2012/8759/
 - Kraft, P., Vache, K. B., Frede, H.-G. and Breuer, L.: A hydrological programming language extension for integrated catchment models, Environ. Model. Softw., 26, 828-830, doi:10.1016/j.envsoft.2010.12.009, 2011.
 - Qu, Y.Z., Duffy, C.J., 2007. A semidiscrete finite volume formulation for multiprocess watershed simulation. Water Resour.Res. 43, W08419, doi:10.1029/2006WR005752.
 - Seibert, J., McDonnell, J.J., 2002. On the dialog between experimentalist and modeler in catchment hydrology: Use of soft data for multicriteria model calibration. Water Resour.Res. 38, doi: 10.1029/2001WR000978.
 - Sivapalan, M., 2003. Process complexity at hillslope scale, process simplicity at the watershed scale: is there a connection? Hydrol.Proc. 17, 1037-1041.
 - Tetzlaff, D., McDonnell, J.J., Uhlenbrook, S., McGuire, K.J., Bogaart, P.W., Naef, F., Baird, A.J., Dunn, S.M., Soulsby, C., 2008. Conceptualizing catchment processes: simply too complex? Hydrol.Proc. 22, 1727-1730.
 - Vache, K.B., McDonnell, J.J., 2006. A process-based rejectionist framework for evaluating catchment runoff model structure. Water Resour.Res. W02409, doi:10.1029/2005WR004247.
