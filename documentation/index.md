@mainpage

## What is cmf?

cmf is a __programming library__ to create __hydrological models__,
which are highly modular and connectible to other models developed using
a @subpage cmfIntroduction "multiple hypotheses background".
The library provides a programming interface consisting using
an @subpage cmfSoftwareObjects "object orientated approach". 

Although written in C++, its primary usage is to be compiled as an extension to other
programming languages, using [SWIG](http://www.swig.org). Until now only
[Python](http://www.python.org) is actively supported to use the
@subpage cmfSoftwareObjects "objects" of cmf to set up a scope dependend, 
specialized model. cmf uses the @subpage finiteVolumeMethod "Finite Volume Method" 
to set up a wide range of models of water flow through your study area.

It is published as __free software__ under
[GPLv3](http://www.gnu.org/licenses/gpl.html). (c) 2007-2018 by [Philipp Kraft](https://philippkraft.github.io)
and [Institute of Landscape Ecology and Resources Management, Justus-Liebig Universität
Gießen](https://www.jlug.de/hydro).

The development takes place at [GitHub](https://github.com/philippkraft/cmf) 

![](storages_web.png)

## Documentation

  - @subpage tutorial "Tutorial"
  - @subpage publicationList "Publications"
  - @subpage cmfInstall "Installation"
  - @subpage cmfRecommendedSoftwareEnvironment "Recommended Software"


## Get cmf

### Source code

The code development and the download of the cmf source code has been
moved to github:

https://github.com/philippkraft/cmf

And source releases are hosted on PyPI:

https://pypi.python.org/pypi/cmf 

### Precompiled binaries for Windows are available on PyPI:

https://pypi.python.org/pypi/cmf
