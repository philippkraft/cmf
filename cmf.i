// Include datetime
// %pythoncode
// {
// import datetime
// }
// Get Documentation
%include "docstrings.i"
%feature("autodoc","1") ;

// Usage for automated downcast
%include factory.i

// Include typemaps for STL
%include "stl.i"
%include "std_set.i"

// enable exception support
%include "exception.i"
%exception {
    try {
        $action
    } 
    SWIG_CATCH_STDEXCEPT
}
%include "attribute.i"

#define SWIG_SHARED_PTR_NAMESPACE std
#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include <boost_shared_ptr.i>

// Makro to extend base classes with downcasts to child classes
%define DOWNCASTdefinition(BaseClass,ChildClass,funcname)
%extend BaseClass
{
	ChildClass* funcname()
	{
	  return dynamic_cast<ChildClass*>($self);
	}
}
%enddef


// Start my Module
%module cmf_core

%include "geometry/geometry.i"

%echo "%include geometry/geometry.i";

%include "math/math.i"

%echo "math.i OK";

%include "water/water.i"
%echo "water.i OK";

%include "Atmosphere/Meteorology.i"
%echo "Atmosphere OK!";


%{
	// Include Upslope
	#include "upslope/Soil/RetentionCurve.h"
	#include "upslope/SoilWaterStorage.h"
%}
%echo "Cell..";
%include "Upslope/cell.i"
%{
	#include "upslope/connections/subsurfacefluxes.h"
	#include "upslope/connections/surfacefluxes.h"
	#include "upslope/connections/atmosphericfluxes.h"
	#include "upslope/connections/infiltration.h"
	#include "upslope/vegetation/ET.h"
	// Include river model
	#include "Reach/ReachType.h"
	#include "Reach/VolumeHeightRelation.h"
	#include "Reach/ManningConnection.h"
	#include "Reach/reach.h"
	#include "project.h"
	// Include the combined solver
	// Include Factories and helper IO
%}




// Get Upslope Classes
%implicitconv cmf::upslope::RCurve;
%include "upslope/soil/RetentionCurve.h"
%extent cmf::upslope::BrooksCoreyRetentionCurve
{
    %pythoncode {
    def __repr__(self):
        return "Brooks-Corey (Ksat=%g,porosity=%g,b=%g,wetness @ h=%g @ %g)" % (self.K(1,0),self.Porosity(0),self.b(),self.wetness_X,self.Psi_X)
    }
}
%extent cmf::upslope::VanGenuchtenMualem
{
    %pythoncode {
    def __repr__(self):
        return "VanGenuchten-Mualem (Ksat=%g,porosity=%g,alpha=%g, n=%g)" % (self.K(1,0),self.Porosity(0),self.alpha,self.n)
    }
}

%immutable cmf::upslope::SoilWaterStorage::cell;
%include "upslope/SoilWaterStorage.h"
%extend cmf::upslope::SoilWaterStorage {
  %pythoncode {
    boundary=property(lambda self:(self.LowerBoundary(),self.UpperBoundary()),None,"Returns the upper and lower boundary of the layer")
    def __repr__(self):
        return self.Name
  }
}


%include "upslope/connections/subsurfacefluxes.h"
%include "upslope/connections/surfacefluxes.h"
%include "upslope/connections/atmosphericfluxes.h"
%include "upslope/connections/infiltration.h"
%include "upslope/vegetation/ET.h"

%pythoncode
{
    def connect(type,left_node,right_node,*args):
        connection=type(left_node,right_node,*args)
        connection.thisown=0
}
%echo "upslope::connections OK!";

// Get river model classes
%include "Reach/ReachType.h"
%include "Reach/VolumeHeightRelation.h"
%newobject cmf::water::OpenWaterStorage::FromNode;
%include "Reach/ManningConnection.h"

%attribute2(cmf::river::Reach,cmf::upslope::Cell,cell,get_cell);
%attribute(cmf::river::Reach,cmf::river::Reach*,downstream,get_downstream);
%attribute(cmf::river::Reach,cmf::river::Reach*,root,get_root);
%attribute2(cmf::river::Reach,cmf::river::OpenWaterStorage,water,get_water);
%attribute(cmf::river::Reach,cmf::water::FluxNode*,outlet,get_outlet);
%attribute(cmf::river::Reach,int,upstream_count,upstream_count);
%attribute(cmf::river::Reach,real,depth,get_depth,set_depth);

%attribute(cmf::river::ReachIterator,cmf::river::Reach*,reach,reach);
%attribute(cmf::river::ReachIterator,double,position,position);

%include "Reach/reach.h"
%extend cmf::river::Reach {
%pythoncode {
    @property
    def upstream(self):
        """Returns a list containing all reaches flowing into self"""
        return [self.get_upstream(i) for i in range(self.upstream_count)]
    def __hash__(self):
        return hash(self.water.node_id)
    def __repr__(self):
        return "A reach (node #%i) at %s" % (self.water.node_id,self.cell)
       
}}
%extend cmf::river::ReachIterator {
%pythoncode {
    def __iter__(self):
        while self.valid():
            self.next()
            yield (self.reach,self.position)
}}
%echo "cmf::river OK!";

// Project
%rename(connect_cells_with_flux) cmf::connect_cells_with_flux;
%include "project.h"
%extend cmf::project {
%pythoncode {
    @property
    def cells(self):
        for i in range(self.CellCount()):
            yield self.Cell(i)
    @property 
    def boundary_conditions(self):
        it=bc_iterator(self)
        while it.valid():
            yield it.next()
    def __iter__(self):
        return self.cells
    def __len__(self):
        return self.CellCount()
    def __getitem__(self,index):
        if isinstance(index,slice):
            return [self.Cell(i) for i in range(*index.indices(len(p)))]
        return self.Cell(index)
    def cell_list(self,expression='True'):
        res=[]
        f=lambda cell:eval(expression)
        for cell in self.cells():
            if f(cell):
                res.append(cell)
        return res
}}

%echo "cmf::project OK!";


%pythoncode {
    def get_layers(cells):
        for c in cells:
            for l in c:
               yield l
    def count_layers(cells):
        res=0
        for c in cells:
            res+=c.LayerCount()
        return res
    def query_layers(layers,expr='layer.theta()'):
        f=lambda layer:eval(expr)
        for l in layers:
            yield f(l)
}            





