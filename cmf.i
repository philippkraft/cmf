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
%module cmf

%include "geometry/geometry.i"
%include "math/math.i"
%include "geometry/maps.i"

%include "water/water.i"

%{
	#include "Atmosphere/Meteorology.h"
	#include "Atmosphere/Precipitation.h"


	// Include Upslope
	#include "upslope/Soil/RetentionCurve.h"
	#include "upslope/SoilWaterStorage.h"
	#include "upslope/Vegetation/StructVegetation.h"
	#include "upslope/cell.h"
	#include "upslope/topology.h"
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



%include "Atmosphere/Meteorology.h"
%include "Atmosphere/Precipitation.h"


// Get Upslope Classes
%implicitconv cmf::upslope::RCurve;
%include "upslope/soil/RetentionCurve.h"
%extend cmf::upslope::Profile
{%pythoncode{
    def __getitem__(self,index):
        return (self.r_curve(index),self.upperboundary(index),self.lowerboundary(index))
    def __iter__(self):
        for i in range(self.size()):
            yield self[i]
}}

%immutable cmf::upslope::SoilWaterStorage::cell;
%include "upslope/SoilWaterStorage.h"
%extend cmf::upslope::SoilWaterStorage {
  %pythoncode {
    boundary=property(lambda self:(self.LowerBoundary(),self.UpperBoundary()),None,"Returns the upper and lower boundary of the layer")
  }
}
%include "upslope/Vegetation/StructVegetation.h"
%nodefaultctor cmf::upslope::NeighborIterator;
%factory(cmf::water::FluxNode& cmf::upslope::Cell::SurfaceWater,cmf::river::OpenWaterStorage, cmf::water::FluxNode);
%factory(cmf::water::WaterStorage& cmf::upslope::Cell::GetStorage,cmf::river::OpenWaterStorage,cmf::water::WaterStorage);
%attribute2(cmf::upslope::Cell,cmf::upslope::Topology,topology,get_topology)
//%attribute2(cmf::upslope::Cell,cmf::water::FluxNode,ET,ET)
%include "upslope/cell.h"
%pythoncode
{
    class _cell_object_list:
        def __init__(self,c,kind):
            self.c=c
            self.kind=kind
        def get(self,index):
            if self.kind=="L":
                return self.c.Layer(index)
            elif self.kind=="S":
                return self.c.GetStorage(index)
            else:
                 ndx=index if index>=0 else self.c.StorageCount()+self.c.LayerCount()+index
                 lndx=index-self.c.StorageCount()
                 if lndx<0:
                     return self.c.GetStorage(ndx)
                 else:
                     return self.c.Layer(lndx)
        def __getitem__(self,index):
            if (type(index)==slice):
                 return list(map(self.get,range(*index.indices(len(self)))))
            try:
                 gen=iter(index)
                 return list(map(self.get,gen))
            except TypeError:
                 return self.get(index)
        def find_by_name(self,name):
            for s in self:
                if s.Name==name:
                    return s
            raise IndexError("No storage %s in %s of %s" % (name,"layers" if self.kind=='L' else "non layer storages" if self.kind=='S' else "storages",self.c))
        def __len__(self):
            return self.c.LayerCount() if self.kind=='L' else (self.c.StorageCount() if self.kind=='S' else self.c.LayerCount()+self.c.StorageCount())
        def __iter__(self):
            for i in range(len(self)):
                yield self[i]
}
%extend cmf::upslope::Cell
{
	std::string __repr__() {return $self->ToString();}
	cmf::upslope::SoilWaterStorage& __getitem__(int i) {return $self->Layer(i);}
%pythoncode
    {
    @property
    def neighbors(self):
        c_iter=NeighborIterator(self)
        while c_iter.valid():
            yield (c_iter.cell(),c_iter.flowwidth())
            c_iter.next()
    storages=property(lambda c:_cell_object_list(c,'A'),None,"Provides access to all storages of the cell (surface storages and layers)")
    surface_storages=property(lambda c:_cell_object_list(c,'S'),None,"Provides access to all surface storages of the cell, like canopy, snow, surface water etc")
    layers=property(lambda c:_cell_object_list(c,'L'),None,"Provides access to all soil water storages (layers) of the cell")
    surface_water=property(SurfaceWater,None,"Gives access to the surface water, which is either a distributing flux node, or the storage for all surface water")
    canopy=property(GetCanopy,None,"The canopy water storage of the cell, if it exists")
    snow=property(GetSnow,None,"The snow pack of the cell, if a storage for the snow exists")
    saturated_depth=property(SaturatedDepth,SetSaturatedDepth,"Gets or sets the saturated depth of a cell, if setting each layer of the cell will get a new water content")
    area=property(Area,None,"The area of the cell in m2")
    soildepth=property(lambda self:0 if self.layercount()==0 else self[-1].boundary[-1],None,"the soildepth in m (lower boundary of lowest layer)")
    reach=property(lambda self:self.GetReach(0),None,"The first reach of the cell, other reaches are accessible via reaches[n]")
    reaches=property(lambda self:[self.GetReach(i) for i in range(self.ReachCount())],None,"The reaches of this cell")
    def connect_soil_with_node(self,node,type,flowwidth,distance,upper_boundary=0,lower_boundary=None):
        """Connects all layers between the boundaries with a node using a flux connection
        node: Target node (FluxNode)
        type: Type of the connection (e.g. cmf.Richards_lateral)
        flowwidth: Width of the connection
        distance: distance of the connection
        upper_boundary: Connect only layers, whose lower depth is greater then this value
        lower_boundary: Connect only  layers, whose upper depth is smaller then this value
        """
        if lower_boundary is None:
            lower_boundary=self.soildepth
        for l in self:
            if l.boundary[0]<lower_boundary and l.boundary[1]>upper_boundary:
                type(l,node,flowwidth,distance).thisown=0
    def __hash__(self):
        return hash((self.x,self.y,self.z,self.area,self.Id))
    def __eq__(self,cmp):
        return hash(self)==hash(cmp)
    }
}
%include "upslope/topology.h"
%extend cmf::upslope::NeighborIterator
{
	bool __eq__(const NeighborIterator& cmp) {return (*$self)==cmp;}
	bool __neq__(const NeighborIterator& cmp) {return (*$self)!=cmp;}
}

%template(cell_vector) std::vector<cmf::upslope::Cell*>;

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

//%clearnodefaultctor;
// Get maps
ComplexMAP(cmf::upslope::vegetation::Vegetation,Vegetation)
ASMAPFunction(cmf::upslope::vegetation::Vegetation,Vegetation)
ComplexMAP(cmf::upslope::RCurve,RetentionCurve)
ASMAPFunction(cmf::upslope::RCurve,RetentionCurve)
ComplexMAP(cmf::upslope::Profile,Profile)
ASMAPFunction(cmf::upslope::Profile,Profile)


// Get river model classes
%include "Reach/ReachType.h"
%include "Reach/VolumeHeightRelation.h"
%newobject cmf::water::OpenWaterStorage::FromNode;
%include "Reach/ManningConnection.h"
%include "Reach/reach.h"
%extend cmf::river::Reach {
%pythoncode {
   @property
   def upstream(self):
       """Returns a generator to iterate over each reach upstream from self, inlcuding self"""
       return [self.get_upstream(i) for i in range(self.upstream_count)]
}}
%extend cmf::river::ReachIterator {
%pythoncode {
    def __iter__(self):
        while self.valid():
            yield self.next()
}}
// Project
%include "project.h"
%extend cmf::project {
%pythoncode {
    @property
    def cells(self):
        for i in range(self.CellCount()):
            yield self.Cell(i)
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





