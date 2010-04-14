

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
%{
	#include "reach/ReachType.h"
	#include "reach/OpenWaterStorage.h"
	#include "reach/Reach.h"
%}

// Get river model classes
%include "reach/ReachType.h"

%attribute(cmf::river::OpenWaterStorage, real,depth,get_depth,set_depth);

%include "reach/OpenWaterStorage.h"

%attributeval(cmf::river::Reach,cmf::upslope::cell_vector,cells,get_cells);
%attribute(cmf::river::Reach,cmf::river::Reach,downstream,get_downstream);
%attribute(cmf::river::Reach,cmf::river::Reach,root,get_root);
%attribute(cmf::river::Reach,int,upstream_count,upstream_count);

%attribute(cmf::river::ReachIterator,cmf::river::Reach,reach,reach);

%attribute(cmf::river::ReachIterator,double,position,position);

%include "reach/Reach.h"
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
