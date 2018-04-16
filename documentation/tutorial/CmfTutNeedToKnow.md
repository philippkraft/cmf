@page cmfTutNeedToKnow What you need to know for using cmf

First, you need to have some knowledge of Python - how to write loops,
functions, perform math etc. Python is a programming language gaining
influence for scientific computing since years, even Nature wrote aboute
it
[(Perkel 2015)](http://www.nature.com/news/programming-pick-up-python-1.16833).
You should know how to read and write data from file. If you have
already some programming skills, but you are new to Python, please read
first the official [Python
tutorial](http://docs.python.org/3/tutorial/), especially section 1-10.
A second good source is the freely available book [Python in
Hydrology](http://www.greenteapress.com/pythonhydro/pythonhydro.html),
as a compagnion to the official tutorial.

To see what happens some knowledge of [NumPy](http://numpy.org) and
[MatPlotLib](http://matplotlib.org) is very helpful, to get a visual
output of your results.

If you are completely new to programming, using cmf will be quite hard,
but you don't need to be the super-hacker. If cmf is for you an argument
to learn programming with Python (you will have use for that beyond cmf.
This wiki and its webserver for instance is written in Python) you might
like to have a look at these free books: [Think Python
(3)](http://greenteapress.com/wp/think-python-2e/) or [Learn Python (2)
the hard way](http://learnpythonthehardway.org/book/).

## Python 2 vs. 3

The programming language Python has undergone some changes between the
version 2 and 3. Although Python 3 is available since 2008, it took
quite a while until all major libraries were compatible to Python 3.
Since 2014 this is the case and cmf is available for Python 3 also. I
recommend to use it, since cmf will not be available for Python 2 on
Windows by 2018. However, Python 2 is still around. The tutorials are
made to run in both versions.

## How to deal with the tutorials

The best way to try out this tutorial is from an interactive environment
line, like IPython or Spyder (see CmfRecommendedSoftwareEnvironment).
The tutorials do not include excercises. However, to understand what is
going on, try to understand every line you type or copy (better to type)
and make changes to the code to see what happens. Change parameters,
extend the problem and so on. For the later tutorials it is a good
exercise to combine tutorials, eg. write a 1D model with real wheater
data on your own, rather than download it directly.
