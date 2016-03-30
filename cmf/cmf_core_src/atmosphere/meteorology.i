

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
	#include "atmosphere/Weather.h"
	#include "atmosphere/meteorology.h"
	#include "atmosphere/precipitation.h"
%}

%include "Weather.h"

%extend cmf::atmosphere::Weather {
    %pythoncode {
    def __repr__(self):
        return "cmf.Weather(Tmin=%0.2f,Tmax=%0.2f)" % (self.Tmin,self.Tmax)
    def __str__(self):
        return self.to_string()
}}


%shared_ptr(cmf::atmosphere::MeteoStation)
%shared_ptr(cmf::atmosphere::RainfallStation)
%shared_ptr(cmf::atmosphere::RainSource);
%shared_ptr(cmf::atmosphere::RainfallStationReference);
%shared_ptr(cmf::atmosphere::IDWRainfall);
%shared_ptr(cmf::atmosphere::ConstantRainSource);
%shared_ptr(cmf::atmosphere::TimeseriesRainSource);


%rename(__getitem__) cmf::atmosphere::MeteoStationList::operator[];
%rename(__len__) cmf::atmosphere::MeteoStationList::size;
%include "atmosphere/meteorology.h"


%extend cmf::atmosphere::MeteoStationList {
    %pythoncode {
    def __iter__(self):
        for i in xrange(len(self)):
            yield self[i]
    def __repr__(self):
        return "list of %i cmf meteorological stations" % len(self)
    }
}    
    
%extend cmf::atmosphere::MeteoStation {
	%pythoncode
    {
    def TimeseriesDictionary(self):
        return {"Tmin":self.Tmin,
                "Tmax":self.Tmax,
                "Tdew":self.Tdew,
                "T":self.T,
                "rHmean":self.rHmean,
                "rHmax":self.rHmax,
                "rHmin":self.rHmin,
                "Sunshine":self.Sunshine,
                "Windspeed":self.Windspeed,
                "Rs" : self.Rs}
    def __repr__(self):
        return "cmf.MeteoStation(%s,lat=%0.5g,lon=%0.5g,z=%6.1f)" % (self.Name,self.Latitude,self.Longitude,self.z)
    }
}
%rename(__repr__) cmf::atmosphere::RainfallStation::tostring;
%extend cmf::atmosphere::RainfallStation {
    double __call__(cmf::math::Time t) const { return $self->data[t]; }
}
%rename(__getitem__) cmf::atmosphere::RainfallStationList::operator[];
%rename(__len__) cmf::atmosphere::RainfallStationList::size;
%rename(__call__) cmf::atmosphere::RainSource::operator();
%include "precipitation.h"

%extend cmf::atmosphere::RainfallStationList {
    %pythoncode {
    def __repr__(self):
        return repr(list(self))
    def __iter__(self):
        for i in range(len(self)):
            yield self[i]
    }
}
    
%extend__repr__(cmf::atmosphere::RainSource)
%extend__repr__(cmf::atmosphere::RainfallStationReference)
%extend__repr__(cmf::atmosphere::ConstantRainSource)
%extend__repr__(cmf::atmosphere::IDWRainfall)
