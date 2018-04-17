@page descriptor CMF Descriptor

When creating and using CMF models it can sometimes become quite
confusing due to the large amount of code, especially in more complex
models. This way mistakes can easily slip through. To help avoid this
CMF has a describe function, which outputs a description of the whole
model (or parts of it).

## Usage

First we need to create a simple CMF model.

~~~~~~~~~~~~~{.py}

import cmf
p = cmf.project()
c = p.NewCell(0,0,0,1000)
~~~~~~~~~~~~~

Add a layer, a outlet and a connection

~~~~~~~~~~~~~{.py}

l1 = c.add_layer(1)
out = p.NewOutlet("out", 0, 0,0)
cmf.kinematic_wave(l1, out, 1)
~~~~~~~~~~~~~

Now we can let CMF describe the whole model or parts of it.

~~~~~~~~~~~~~{.py}
print(cmf.describe(p))
print(cmf.describe(l1))
~~~~~~~~~~~~~

Conveniently we can even have the output written to a file, which we
have defined beforehand.

~~~~~~~~~~~~~{.py}

file = open("out.txt", "w")
cmf.describe(p, out=file)
~~~~~~~~~~~~~

# Fluxograms

Fluxograms are a novel way to get a nice animation of your model. Sadly,
to implement them is a bit complicated. But if you still want to try it
a tutorial can be found here:
<http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/fluxogram_and_get_fluxes>


