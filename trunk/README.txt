Requirements:

 - Python 2.6 (www.python.org), python x,y 2.6.2 recommended (pythonxy.com),  
 - shapely 1.0 (http://pypi.python.org/pypi/Shapely/) 
 - Sundials 2.4 (https://computation.llnl.gov/casc/sundials/download/download.html) 
 - C++ compiler with TR1 support (e.g. gcc 4.4) 
 / or a C++ compiler and the Boost library (www.boost.org). 
 / Boost 1.40 works well, older versions *may* work also

Sundials has to be compiled with the same compiler as you are using for cmf

Open setup.py in a text editor and change the first lines to match your installation.
After saving changes you can use can run 

c:\>python setup.py install

from a command line in Windows or

$ python setup.py intall

in *NIX system (not tested yet)

for installing cmf on your system. Eventually adminstrative rights are needed.

If anything fails, please write to:

Philipp Kraft

philipp.kraft@umwelt.uni-giessen.de