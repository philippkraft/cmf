SWIG_SHARED_PTR(MeteoStation,cmf::atmosphere::MeteoStation)

%{
	#include "Atmosphere/Meteorology.h"
	#include "Atmosphere/Precipitation.h"
%}

%rename(__getitem__) cmf::atmosphere::MeteoStationList::operator[];
%rename(__len__) cmf::atmosphere::MeteoStationList::size;
%ignore cmf::atmosphere::meteo_station_pointer;
%include "Atmosphere/Meteorology.h"

%extend cmf::atmosphere::Weather {
    %pythoncode {
    def __repr__(self):
        return "cmf.Weather()"
    def __str__(self):
        return "Weather: T(max/min)=%6.2f(%3.0f/%3.0f), Rs=%7.2f, rH=%3.0f%%" % (self.T,self.Tmin,self.Tmax,self.Rs,100*self.e_a/self.e_s)
}}

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
%include "Precipitation.h"
