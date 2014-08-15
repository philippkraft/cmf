

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   
//SWIG_SHARED_PTR(Cell,cmf::upslope::Cell);

%{
    #include "upslope/vegetation/StructVegetation.h"
    #include "upslope/surfacewater.h"
    #include "upslope/cell.h"
    #include "upslope/Topology.h"
    #include "upslope/algorithm.h"

%}

%include "upslope/vegetation/StructVegetation.h"
%extend cmf::upslope::vegetation::Vegetation {
    std::string __repr__() {
        std::stringstream sstr;
        sstr << "cmf.vegetation(\n" 
             << "                 Height = " << $self->Height << " [m]" << std::endl
             << "                    LAI = " << $self->LAI << " [m2/m2]" << std::endl
             << "                 albedo = " << $self->albedo << " [W/W]" << std::endl
             << "          CanopyClosure = " << $self->CanopyClosure << " [m2/m2]" << std::endl
             << "    CanopyPARExtinction = " << $self->CanopyPARExtinction << std::endl
             << "   CanopyCapacityPerLAI = " << $self->CanopyCapacityPerLAI << " [mm]" << std::endl
             << "     StomatalResistance = " << $self->StomatalResistance << " [s/m]" << std::endl
             << "              RootDepth = " << $self->RootDepth << " [m]" << std::endl
             << "  fraction_at_rootdepth = " << $self->fraction_at_rootdepth << " [-]" << std::endl
             << "            RootContent = " << $self->RootContent << " [kg/m3]" << std::endl
             << ")" << std::endl;
        return sstr.str();
    }
}



//////////////////////////////
// Cell
///////////////////////////////
%nodefaultctor cmf::upslope::NeighborIterator;


%node_downcast(cmf::water::flux_node::ptr cmf::upslope::Cell::get_surfacewater,cmf::upslope::SurfaceWater, cmf::water::DirichletBoundary)
%node_downcast(cmf::water::WaterStorage::ptr cmf::upslope::Cell::get_storage,cmf::river::OpenWaterStorage, cmf::water::WaterStorage)
%node_downcast(cmf::atmosphere::RainSource::ptr cmf::upslope::Cell::get_rain_source,cmf::atmosphere::RainfallStationReference::ptr,cmf::atmosphere::ConstantRainSource::ptr, cmf::atmosphere::IDWRainfall)

%attribute2(cmf::upslope::Cell,cmf::upslope::Topology,topology,get_topology);
%shared_attr(cmf::upslope::Cell,Cell,evaporation,get_evaporation);
%shared_attr(cmf::upslope::Cell,Cell,transpiration,get_transpiration);
%attribute2(cmf::upslope::Cell,cmf::atmosphere::Meteorology,meteorology,get_meteorology,set_meteorology);
%shared_attr(cmf::upslope::Cell, Cell,rain_source,get_rain_source,set_rain_source);
%attribute2(cmf::upslope::Cell,cmf::upslope::layer_list,layers,get_layers);
%attribute2(cmf::upslope::Cell,cmf::project,project,get_project);

%attribute(cmf::upslope::Cell,real,saturated_depth,get_saturated_depth,set_saturated_depth);
%attribute(cmf::upslope::Cell,real,area,get_area);
%attribute(cmf::upslope::Cell,real,soildepth,get_soildepth);
%include "upslope/cell.h"

%extend cmf::upslope::Cell
{
%pythoncode
    {
    @property
    def surface_storages(self):
        "Provides access to all surface storages of the cell, like canopy, snow, surface water etc"
        return [self.get_storage(i) for i in range(self.storage_count())]
    @property
    def storages(self):
        "Provides access to all storages of the cell (surface storages and layers)"
        return self.surface_storages + list(self.layers)
    
    @property
    def neighbors(self):
        c_iter=neighbor_iterator(self)
        while c_iter.valid():
            yield (c_iter.cell(),c_iter.flowwidth())
            c_iter.next()
    
    surfacewater=property(get_surfacewater,None,"Gives access to the surface water, which is either a distributing flux node, or the storage for all surface water")
    canopy=property(get_canopy,None,"The canopy water storage of the cell, if it exists")
    snow=property(get_snow,None,"The snow pack of the cell, if a storage for the snow exists")
        
    contributing_area=property(lambda self:self.topology.ContributingArea(),None,"Contributing area of this cell m2")
    main_outlet=property(lambda self:self.topology.MainOutlet(),None,"The main outlet of the surface water of this cell")
    
    def connect_soil_with_node(self,node,ctype,flowwidth,distance,upper_boundary=0,lower_boundary=None):
        """Connects all layers between the boundaries with a node using a flux connection
        node: Target node (flux_node)
        type: Type of the connection (e.g. cmf.Richards_lateral)
        flowwidth: Width of the connection
        distance: distance of the connection
        upper_boundary: Connect only layers, whose lower depth is greater then this value
        lower_boundary: Connect only  layers, whose upper depth is smaller then this value
        """
        if lower_boundary is None:
            lower_boundary=self.soildepth
        connections=[ctype(l,node,flowwidth,distance) 
                     for l in self.layers 
                     if     l.boundary[0]<lower_boundary 
                        and l.boundary[1]>upper_boundary 
                    ]
   
    def install_connection(self,connection_type):
        """Calls the static use_for_cell method of the connection_type, if present.
        The use_for_cell method might do quite a lot of stuff, please consult the docs for the connection.
        Eg. >>>help(Richards.use_for_cell) 
        """
        if hasattr(connection_type,"use_for_cell"):
            connection_type.use_for_cell(self)
        else:
            raise TypeError("Only connection types implementing a static use_for_cell function can be used")
    def __hash__(self):
        return hash((self.x,self.y,self.Id))
    def __eq__(self,cmp):
        return hash(self)==hash(cmp)
    def __repr__(self):
        return "cell #%i(%g,%g,%g)" % (self.Id,self.x,self.y,self.z)
    }
}
%include "upslope/Topology.h"
%extend cmf::upslope::neighbor_iterator
{
    bool __eq__(const cmf::upslope::neighbor_iterator& cmp) {return (*$self)==cmp;}
    bool __neq__(const cmf::upslope::neighbor_iterator& cmp) {return (*$self)!=cmp;}
}

%iterable_to_list(cmf::upslope::cell_vector,cmf::upslope::Cell);

%rename(__len__) cmf::upslope::cell_vector::size;
%rename(__getitem__) cmf::upslope::cell_vector::operator[];
%rename(__getslice__) cmf::upslope::cell_vector::get_slice;
%rename(__contains__) cmf::upslope::cell_vector::contains;
%attribute2(cmf::upslope::cell_vector,cmf::upslope::Cell,lowest,get_lowest)
%attribute2(cmf::upslope::cell_vector,cmf::upslope::Cell,highest,get_highest)
%include "upslope/cell_vector.h"

%extend cmf::upslope::cell_vector {
%pythoncode {
    def __iter__(self):
        for i in range(len(self)):
            yield self[i]   
    def __repr__(self):
        if len(self):
            return "[%i cells, %0.0fm2, %0.0f - %0.0f m asl.]" % (len(self),self.get_area(),self.lowest.z,self.highest.z)
        else:
            return "<empty cell vector>"        
}}

%include "upslope/algorithm.h"

