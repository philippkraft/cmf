%{
   #include "geometry/maps.h"
%}
%include "geometry/maps.h"

#define WRAPPED_NAME(mapclass,name) mapclass ## name
%define ComplexMAP(TYPE,NAME)
	%template(WRAPPED_NAME(IMap_,NAME)) cmf::maps::IMap<TYPE>;
	%template(WRAPPED_NAME(SingleValueMap_,NAME)) cmf::maps::SingleValueMap<TYPE>;
	%template(WRAPPED_NAME(DiscreteRasterMap_,NAME)) cmf::maps::DiscreteRasterMap<TYPE>;
	%template(WRAPPED_NAME(NearestNeighborMap_,NAME)) cmf::maps::NearestNeighborMap<TYPE>;
	%template(WRAPPED_NAME(PolygonMap_,NAME)) cmf::maps::PolygonMap<TYPE>;
%enddef
%define ScalarMAP(TYPE,NAME)
	%template(WRAPPED_NAME(IMap_,NAME)) cmf::maps::IMap<TYPE>;
	%template(WRAPPED_NAME(SingleValueMap_,NAME)) cmf::maps::SingleValueMap<TYPE>;
	%template(WRAPPED_NAME(ContinuousRasterMap_,NAME)) cmf::maps::ContinuousRasterMap<TYPE>;
	%template(WRAPPED_NAME(DiscreteRasterMap_,NAME)) cmf::maps::DiscreteRasterMap<TYPE>;
	%template(WRAPPED_NAME(NearestNeighborMap_,NAME)) cmf::maps::NearestNeighborMap<TYPE>;
	%template(WRAPPED_NAME(PolygonMap_,NAME)) cmf::maps::PolygonMap<TYPE>;
%enddef
%define ASMAPFunction(TYPE,NAME)
	%extend TYPE {
		cmf::maps::SingleValueMap<TYPE> AsMap() const
		{
			return cmf::maps::SingleValueMap<TYPE>(*$self);
		}
	}
%enddef

ScalarMAP(double,Double)
ScalarMAP(float,Float)
ScalarMAP(int,Int)
ComplexMAP(PyObject*,Py)

