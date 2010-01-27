

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

%include "project.h"
%extend cmf::project {
    %pythoncode {
    def __repr__(self):
        return "cmf.project(%i cells, %i meteo stations, %i outlets)" % (len(self.cells),len(self.meteo_stations),len(self.outlets))
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
    }
}    
    
