SWIG_SHARED_PTR(MeteoStation,cmf::atmosphere::MeteoStation)

%{
	#include "Atmosphere/Meteorology.h"
	#include "Atmosphere/Precipitation.h"
%}

%rename(__getitem__) cmf::atmosphere::MeteoStationList::operator[];
%rename(__len__) cmf::atmosphere::MeteoStationList::size;
%ignore cmf::atmosphere::meteo_station_pointer;
%include "Atmosphere/Meteorology.h"
%extend cmf::atmosphere::MeteoStationList {
    %pythoncode {
    def __iter__(self):
        for i in xrange(len(self):
            yield self[i]
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
                "Prec":self.Prec,
                "rHmean":self.rHmean,
                "rHmax":self.rHmax,
                "rHmin":self.rHmin,
                "Sunshine":self.Sunshine,
                "Windspeed":self.Windspeed,
                "Rs" : self.Rs}
    }
}
