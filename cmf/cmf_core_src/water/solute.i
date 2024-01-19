

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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


%shared_ptr(cmf::water::SoluteReaction);
%shared_ptr(cmf::water::Solute1stOrderReaction);
%shared_ptr(cmf::water::Solute2ndOrderReaction);
%shared_ptr(cmf::water::SoluteConstantFluxReaction);
%shared_ptr(cmf::water::SoluteDecayReaction);
%shared_ptr(cmf::water::SoluteRateReaction);
%shared_ptr(cmf::water::SoluteEquilibriumReaction);
%shared_ptr(cmf::water::SoluteDiffusiveTransport);

%shared_ptr(cmf::water::SoluteStorage);

%{
// Include Water
#include "water/adsorption.h"
// #include "water/freundlich_adsorption.h"
#include "water/Solute.h"
#include "water/reaction.h"
%}

// Include Water
%include "water/adsorption.h"
%include "water/Solute.h"
%extend cmf::water::solute {
        std::string __repr__() { return "[" + $self->Name + "]"; }
        %pythoncode {
            def __hash__(self):
                return hash((type(self), self.Id))
        }
}
%extend cmf::water::solute_vector
{
    size_t __len__() { return $self->size();}
    %pythoncode
    {
        def __iter__(self):
            for i in range(len(self)):
                yield self[i]
        def __repr__(self):
            return repr([s.Name for s in self])
        def __getitem__(self, index):

            if isinstance(index,slice):
                res = " ".join(
                        self.get_solute(i).Name
                        for i in range(*index.indices(len(self)))
                )
                return type(self)(res)
            else:
                try:
                    it=iter(index)
                    res = type(self)()
                    for o in it:
                        res.append(self.get_solute(o))
                    return res
                except:
                    return self.get_solute(index)
    }
}
%extend cmf::water::SoluteTimeseries
{
    cmf::math::timeseries& __getitem__(const cmf::water::solute& solute)
    {
        return (*$self)[solute];
    }
    void __setitem__(const cmf::water::solute& solute,cmf::math::timeseries concentration)
    {
        (*$self)[solute]=concentration;
    }
    size_t __len__() const
    {
        return $self->size();
    }
}




%feature("director") cmf::water::SoluteReaction;
// %rename(__repr__) *::to_string();
%rename(__add_reactance) cmf::water::SoluteRateReaction::add_reactance;
%include "water/reaction.h"


%extend__repr__(cmf::water::SoluteReaction);
%extend__repr__(cmf::water::Solute1stOrderReaction);
%extend__repr__(cmf::water::Solute2ndOrderReaction);
%extend__repr__(cmf::water::SoluteConstantFluxReaction);
%extend__repr__(cmf::water::SoluteDecayReaction);
%extend__repr__(cmf::water::SoluteEquilibriumReaction);
%extend__repr__(cmf::water::SoluteDiffusiveTransport);
%extend__repr__(cmf::water::SoluteRateReaction);

%iterable_to_list(cmf::List<cmf::water::SoluteReaction::ptr>,cmf::water::SoluteReaction::ptr);
%template(SoluteReactionList) cmf::List<cmf::water::SoluteReaction::ptr>;

void attach_reactions_to_waterstorage(
        std::shared_ptr<cmf::water::WaterStorage> waterstorage,
        const cmf::water::SoluteReactionList& reactions);


%extend cmf::water::SoluteRateReaction {
    %pythoncode
    {
        def update(self, reactances):
            if any(not isinstance(s, solute) for s in reactances):
                raise TypeError('All dict keys need to be cmf.solute objects')
            for s, value in reactances.items():
                try:
                    self.__add_reactance(s, *value)
                except TypeError:
                    self.__add_reactance(s, value)
            return self

        def append(self, solute, stoichiometric_coefficient, partial_order=None):
            if partial_order is None:
                self.__add_reactance(solute, stoichiometric_coefficient)
            else:
                self.__add_reactance(solute, stoichiometric_coefficient, partial_order)
            return self

        @classmethod
        def decay(cls, solute, rate):
            return cls(rate).append(solute, -1)

        @classmethod
        def constant_source(cls, solute, rate):
            """
            Creates a constant concentration source

            @param solute: A cmf.solute
            @param rate: The change rate of the concentration in mol/(m³ day)
            """
            return cls(rate).append(solute, 1, 0)

        @classmethod
        def multi(cls, reactances, k_forward, k_back=0.0):
            return cls(k_forward, k_back).update(reactances)
    }
}

%{
#include "water/SoluteStorage.h"
%}
%attribute(cmf::water::SoluteStorage, real, conc, get_conc, set_conc);
%include "water/SoluteStorage.h"
%extend__repr__(cmf::water::SoluteStorage);

