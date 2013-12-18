#include "precipitation.h"
#include "../project.h"
#include "../upslope/cell.h"
#include "../math/num_array.h"
using namespace cmf::atmosphere;
using namespace cmf::geometry;
using namespace cmf::math;
IDWRainfall::IDWRainfall( cmf::project& project, cmf::geometry::point position, double z_weight, double power ) 
: RainSource(project,position)
{
	point p=position;
	const RainfallStationList& stations = project.rainfall_stations;
	// Create a vector of distances to the stations
	num_array dist(stations.size());
	for (size_t i = 0; i < stations.size() ; ++i)
		dist[i] = p.z_weight_distance(stations[i]->Location,z_weight);

	// Calculate the weight of each station using IDW
	num_array weights_ = 1./dist.power(power);
	double weightsum = weights_.sum();
	weights_/=weightsum; // normalize the weights
	// Create a weight map
	for (size_t i = 0; i < stations.size() ; ++i)
		m_weights[stations[i]] = weights_[i];
	Name = "IDW - interpolated rainfall";
}

IDWRainfall::ptr cmf::atmosphere::IDWRainfall::create(cmf::project& project, cmf::geometry::point position, double z_weight, double power )
{
	IDWRainfall* new_obj = new IDWRainfall(project,position,z_weight,power);
	return IDWRainfall::ptr(new_obj);
}

real cmf::atmosphere::IDWRainfall::get_intensity( cmf::math::Time t ) const
{
	double res = 0.0; 
	// Add each station record, weighted by IDW
	for(weight_map::const_iterator it = m_weights.begin(); it != m_weights.end(); ++it)
		res += it->first->data[t] * it->second;
	return res;
}

real cmf::atmosphere::IDWRainfall::conc( cmf::math::Time t, const cmf::water::solute& Solute ) const
{
	double res = 0.0; 
	// Add each station record, weighted by IDW
	for(weight_map::const_iterator it = m_weights.begin(); it != m_weights.end(); ++it)
		res += it->first->concentration.conc(t,Solute) * it->second;
	return res;
}
cmf::atmosphere::RainfallStationReference::ptr cmf::atmosphere::RainfallStationReference::from_nearest_station(cmf::project& project, cmf::geometry::point position, double z_weight )
{
	if (project.rainfall_stations.size() == 0)
		throw std::runtime_error("Can't create RainfallStationReference without rainfall stations");
	double min_dist = position.z_weight_distance(project.rainfall_stations[0]->Location,z_weight);
	RainfallStation::ptr station=project.rainfall_stations[0];
	for (size_t i = 1; i < project.rainfall_stations.size() ; ++i)
	{
		double dist = position.z_weight_distance(project.rainfall_stations[i]->Location,z_weight);
		if (dist<min_dist) {
			dist=min_dist;
			station = project.rainfall_stations[i];
		}
	}
	RainfallStationReference* rfs_r=new RainfallStationReference(project,position,station);
	return ptr(rfs_r);
}

cmf::atmosphere::RainfallStationReference::ptr cmf::atmosphere::RainfallStationReference::from_station_id( cmf::project& project, cmf::geometry::point position, size_t id )
{
	if (id >= project.rainfall_stations.size() )
		throw std::runtime_error("Id out of range");
	return ptr(new RainfallStationReference(project,position,project.rainfall_stations[id]));
}

cmf::atmosphere::RainfallStationReference::RainfallStationReference(cmf::project& project, cmf::geometry::point position, RainfallStation::ptr station ) 
	: RainSource(project,position), m_station(station)
{
	Name = "Rainfall from " + station->name;
}
cmf::atmosphere::ConstantRainSource::ConstantRainSource(cmf::project& _project,cmf::geometry::point location,real _intensity ) 
: RainSource(_project,location), intensity(_intensity), concentrations(_project.solutes.size(),0.0)
{
	std::stringstream sstr;
	sstr << "Constant rainfall of " << _intensity << " mm/day";
	Name = sstr.str();
}


std::string cmf::atmosphere::RainfallStation::tostring() const
{
	std::stringstream sstr;
	sstr << "<RainfallStation '" << name << "' " << data.mean() * 365 << " mm/year>";	
	return sstr.str();
}

cmf::atmosphere::RainfallStation::RainfallStation( const RainfallStation& copy ) 
: id(copy.id), name(copy.name), data(copy.data.copy()), Location(copy.Location)
{

}

cmf::atmosphere::RainfallStation::ptr cmf::atmosphere::RainfallStation::create( size_t Id,std::string Name, cmf::math::timeseries Data, cmf::geometry::point position )
{
	RainfallStation* new_rfs = new RainfallStation(Id,Name,Data,position);
	return ptr(new_rfs);
}

void cmf::atmosphere::RainfallStation::use_for_cell( cmf::upslope::Cell& c )
{
	c.set_rain_source(RainfallStationReference::from_station_id(c.get_project(), c.get_position(), this->id));
}
RainfallStation::ptr cmf::atmosphere::RainfallStationList::operator[]( ptrdiff_t index )
{
	return m_stations.at(size_t(index >= 0 ? index : ptrdiff_t(size()) + index));
}

const RainfallStation::ptr cmf::atmosphere::RainfallStationList::operator[]( ptrdiff_t index ) const
{
	return m_stations.at(size_t(index >= 0 ? index : ptrdiff_t(size()) + index));
}