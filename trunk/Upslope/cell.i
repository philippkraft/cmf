%pythoncode
{
    class _cell_object_list:
        def __init__(self,c,kind):
            self.c=c
            self.kind=kind
        def __get(self,index):
            if self.kind=="L":
                return self.c.get_layer(index)
            elif self.kind=="S":
                return self.c.get_storage(index)
            else:
                 ndx=index if index>=0 else self.c.storage_count()+self.c.layer_count()+index
                 lndx=index-self.c.storage_count()
                 if lndx<0:
                     return self.c.get_storage(ndx)
                 else:
                     return self.c.get_layer(lndx)
        def __getitem__(self,index):
            if (type(index)==slice):
                 return list(map(self.get,range(*index.indices(len(self)))))
            try:
                 gen=iter(index)
                 return list(map(self.__get,gen))
            except TypeError:
                 return self.__get(index)
        def find_by_name(self,name):
            for s in self:
                if s.Name==name:
                    return s
            raise IndexError("No storage %s in %s of %s" % (name,"layers" if self.kind=='L' else "non layer storages" if self.kind=='S' else "storages",self.c))
        def __len__(self):
            return self.c.layer_count() if self.kind=='L' else (self.c.storage_count() if self.kind=='S' else self.c.layer_count()+self.c.storage_count())
        def __iter__(self):
            for i in range(len(self)):
                yield self[i]
}



%{
	#include "upslope/Vegetation/StructVegetation.h"
	#include "upslope/cell.h"
	#include "upslope/topology.h"
	#include "upslope/algorithm.h"
%}

%include "upslope/Vegetation/StructVegetation.h"
%nodefaultctor cmf::upslope::NeighborIterator;


%factory(cmf::water::FluxNode& cmf::upslope::Cell::get_surfacewater,cmf::river::OpenWaterStorage, cmf::water::FluxNode);
%factory(cmf::water::WaterStorage& cmf::upslope::Cell::get_storage,cmf::river::OpenWaterStorage,cmf::water::WaterStorage);

%attribute2(cmf::upslope::Cell,cmf::upslope::Topology,topology,get_topology);
%attribute2(cmf::upslope::Cell,cmf::water::FluxNode,evaporation,get_evaporation);
%attribute2(cmf::upslope::Cell,cmf::water::FluxNode,transpiration,get_transpiration);
%attribute2(cmf::upslope::Cell,cmf::atmosphere::Meteorology,meteorology,get_meteorology,set_meteorology);
%attribute2(cmf::upslope::Cell,cmf::atmosphere::RainCloud,rain_cloud,get_rainfall);


%include "upslope/cell.h"

%extend cmf::upslope::Cell
{
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
    surface_water=property(get_surfacewater,None,"Gives access to the surface water, which is either a distributing flux node, or the storage for all surface water")
    canopy=property(get_canopy,None,"The canopy water storage of the cell, if it exists")
    snow=property(get_snow,None,"The snow pack of the cell, if a storage for the snow exists")
    saturated_depth=property(get_saturated_depth,set_saturated_depth,"Gets or sets the saturated depth of a cell, if setting each layer of the cell will get a new water content")
    area=property(get_area,None,"The area of the cell in m2")
    soildepth=property(lambda self:0 if self.layer_count()==0 else self[-1].boundary[-1],None,"the soildepth in m (lower boundary of lowest layer)")
    reach=property(lambda self:self.get_reach(0),None,"The first reach of the cell, other reaches are accessible via reaches[n]")
    reaches=property(lambda self:[self.get_reach(i) for i in range(self.ReachCount())],None,"The reaches of this cell")
    Vegetation=property(get_vegetation,set_vegetation,"The vegetational parameters of the cell")
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
    def __repr__(self):
        return "cell #%i(%g,%g,%g)" % (self.Id,self.x,self.y,self.z)
    }
}
%include "upslope/topology.h"
%extend cmf::upslope::NeighborIterator
{
	bool __eq__(const NeighborIterator& cmp) {return (*$self)==cmp;}
	bool __neq__(const NeighborIterator& cmp) {return (*$self)!=cmp;}
}

%template(cell_vector) std::vector<cmf::upslope::Cell*>;

%include "upslope/algorithm.h"
%echo "Cell OK!";

