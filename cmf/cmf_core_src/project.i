

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
// Project
%{
	#include "project.h"
%}

%rename(connect_cells_with_flux) cmf::connect_cells_with_flux;
%attribute2(cmf::project,cmf::upslope::cell_vector,cells,get_cells);
%pythoncode {
    class project_list_wrapper:
        def __init__(self,getitem,getlen,name):
            self.__get = getitem
            self.__len = getlen
            self.name = name
        def __len__(self):
            return int(self.__len())
        def __getitem__(self,index):
            if (type(index)==slice):
                 return [self.__get(i) for i in range(*index.indices(len(self)))]
            try:
                 gen=iter(index)
                 return [self.__get(it) for it in gen]
            except TypeError:
                 return self.__get(index)
        def __iter__(self):
            for i in range(len(self)):
                yield self.__get(i)
        def __repr__(self):
            return '[%i %s of project]' % (len(self),self.name)

}
%include "project.h"
%extend cmf::project {
    %pythoncode {
    def __repr__(self):
        return "cmf.project(%i cells, %i meteo stations, %i project nodes)" % (len(self.cells),len(self.meteo_stations),len(self.nodes))
    def __len__(self):
        return self.size()
    def __getitem__(self,index):
        if isinstance(index,slice):
            return [self[i] for i in range(*index.indices(len(self)))]
        else:
            return self.get_cell(index)        
    def __iter__(self):
        for i in range(self.size()):
            yield self.get_cell(i)
    @property
    def reaches(self):
        "Returns the reaches of the project"
        return project_list_wrapper(self.get_reach,self.reach_count,'reaches')
    @property
    def nodes(self):
        "Returns the nodes of the project"
        return project_list_wrapper(self.get_node,self.node_count,'nodes')
    }
}    
    
