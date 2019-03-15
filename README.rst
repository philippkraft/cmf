|logo|

|lic| |zeno| |pypi| |travis| |appveyor|

.. |logo| image:: https://raw.githubusercontent.com/philippkraft/cmf/master/documentation/media/cmf-logo-klein-2018.png
    :target: https://philippkraft.github.io/cmf

.. |lic| image:: https://raw.githubusercontent.com/philippkraft/cmf/master/documentation/media/license-GPLv3-green.svg?sanitize=true
    :target: https://opensource.org/licenses/GPL-3.0
    
.. |zeno| image:: https://zenodo.org/badge/DOI/10.5281/zenodo.1125290.svg
   :target: https://doi.org/10.5281/zenodo.1125290
   
.. |pypi| image:: https://badge.fury.io/py/cmf.svg
    :target: https://pypi.python.org/pypi/cmf/

.. |travis| image:: https://api.travis-ci.org/philippkraft/cmf.svg 
    :target: https://travis-ci.org/philippkraft/cmf

.. |appveyor| image:: https://ci.appveyor.com/api/projects/status/24crp4ejkacykkrk?svg=true
    :target: https://ci.appveyor.com/project/philippkraft/cmf


Modelling water and solute fluxes
===================================

cmf is a programming library to create hydrological models, which are highly modular and connectible to other
models developed using a `multiple hypotheses background <https://philippkraft.github.io/cmf/cmf_introduction.html>`_
and is based on the `finite volume method <https://philippkraft.github.io/cmf/finite_volume_method.html>`_
Although written in C++, its primary usage is to be compiled
as an extension to other programming languages, using SWIG. Researchers can build individual models,
targeting their scientific question by using the library objects like water storages, boundary conditions,
fluxes and solvers. cmf uses the finite volume method to set up a wide range of models of water flow through
your study area. Resulting models can range from **lumped conceptual models** to **fully distributed darcian models**
and everything in between.

*cmf* is published as free software under GPLv3. (c) 2007-2017 by Philipp Kraft and the
Institute of Landscape Ecology and Resources Management, Justus-Liebig Universität Gießen.

Documentation
---------------
Can be found here: https://philippkraft.github.io/cmf

Development
--------------
You can join the development or report bugs at Github:

.. image:: https://img.shields.io/github/release/philippkraft/cmf.svg?logo=github
    :target: https://github.com/philippkraft/cmf


Publications
------------

The basic publication of cmf is:
    Kraft, P., Vaché, K.B., Frede, H.-G. Breuer, L. 2011. A hydrological programming language extension for integrated
    catchment models, Environmental Modelling & Software, `doi: 10.1016/j.envsoft.2010.12.009 <https://doi.org/10.1016/j.envsoft.2010.12.009>`_


`Published applications <https://philippkraft.github.io/cmf/publication_list.html>`_





