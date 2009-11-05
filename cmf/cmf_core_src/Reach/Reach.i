%{
	#include "Reach/ReachType.h"
	#include "Reach/OpenWaterStorage.h"
	#include "Reach/reach.h"
%}

// Get river model classes
%include "Reach/ReachType.h"

%attribute(cmf::river::OpenWaterStorage, real,depth,get_depth,set_depth);

%include "Reach/OpenWaterStorage.h"

%attributeval(cmf::river::Reach,cmf::upslope::cell_vector,cells,get_cells);
%attribute(cmf::river::Reach,cmf::river::Reach,downstream,get_downstream);
%attribute(cmf::river::Reach,cmf::river::Reach,root,get_root);
%attribute(cmf::river::Reach,int,upstream_count,upstream_count);

%attribute(cmf::river::ReachIterator,cmf::river::Reach,reach,reach);

%attribute(cmf::river::ReachIterator,double,position,position);

%include "Reach/reach.h"
%extent cmf::river::Reach {
%pythoncode {
    @property
    def upstream(self):
        """Returns a list containing all reaches flowing into self"""
        return [self.get_upstream(i) for i in range(self.upstream_count)]
    def __hash__(self):
        return hash(self.water.node_id)
}}
%extent cmf::river::ReachIterator {
%pythoncode {
    def __iter__(self):
        while self.valid():
            self.next()
            yield (self.reach,self.position)
}}

EXTENT__REPR__(cmf::river::OpenWaterStorage)
EXTENT__REPR__(cmf::river::Reach)
