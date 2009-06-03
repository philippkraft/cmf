// Project
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
    
