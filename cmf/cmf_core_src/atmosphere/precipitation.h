

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
#ifndef Precipitation_h__
#define Precipitation_h__
#include "../water/flux_node.h"
#include "../water/boundary_condition.h"
#include "../water/Solute.h"
#include <tr1/memory>

namespace cmf { 
	class project;
	namespace upslope{
		class Cell;
	}
	namespace atmosphere {
		/// @ingroup boundary meteo
		/// An abstract class for different types of rainfall sources
		class RainSource : public cmf::water::flux_node 
		{
		protected:
			/// protected c'tor
			RainSource(const cmf::project& _project,cmf::geometry::point location=cmf::geometry::point())
				: flux_node(_project,location) {}
		public:
			/// shared pointer
			typedef std::tr1::shared_ptr<RainSource> ptr;
			/// Functor declartion RainSource(t) = RainSource.get_intensity(t)
			real operator()(cmf::math::Time t) const { return get_intensity(t);}
			/// Returns the actual rainfall intensity in mm/day
			virtual real get_intensity(cmf::math::Time t) const=0;
			/// Returns the concentration of a solute in the rainfall at time t
			virtual real conc(cmf::math::Time t, const cmf::water::solute& Solute)=0;
			
			virtual bool RecalcFluxes(cmf::math::Time t) {return true;}
			/// Returns false
			virtual bool is_empty() const
			{
				return false;
			}
			


		};
		/// @ingroup boundary meteo
		/// A simple implementation of RainSource. Returns intensity for any time step.
		class ConstantRainSource : public RainSource {
			cmf::math::num_array concentrations;
		public:
			/// shared pointer
			typedef std::tr1::shared_ptr<ConstantRainSource> ptr;

			/// The rain fall intensity in mm/day. This value is returned by 
			real intensity;
			
			/// Creates a new ConstantRainSource. Consider using Cell::set_rainfall 
			/// for internal creation of a constant rain source, instead of direct use
			/// @param _project The project the rain source is belonging to.
			/// @param location The location of the rain source
			/// @param _intensity The constant rainfall intensity in mm/day
			ConstantRainSource(const cmf::project& _project,cmf::geometry::point location,real _intensity);
			real get_intensity(cmf::math::Time t) const {
				return intensity;
			}
			virtual real conc(cmf::math::Time t, const cmf::water::solute& Solute) {
				return concentrations[Solute.Id];
			}
			/// Sets the concentration of a solute in the rainfall
			void set_conc(const cmf::water::solute& Solute, real value) {
				concentrations[Solute.Id] = value;
			}

		};
		
		class RainfallStationList;
		/// @ingroup meteo
		/// RainfallStation describes a rainfall timeseries in mm/day at a certain place. 
		/// Use RainfallStationReference or IDWRainfall to distribute the data into space
		class RainfallStation  {
			RainfallStation(size_t Id,std::string Name, cmf::math::timeseries Data, cmf::geometry::point position)
				: name(Name),data(Data.copy()),Location(position), id(Id) {}
		public:
			typedef std::tr1::shared_ptr<RainfallStation> ptr;
			/// Location of the station
			cmf::geometry::point Location;

			/// Creates a new RainfallStation. Is called by RainfallStationList::add. 
			/// Using RainfallStationList::add is in most cases the better choice, eg. from Python
			/// @code project.rainfall_stations.add("Name", cmf.timeseries.from_file('rainfall.bin'), (65124,78009,187)) @endcode
			/// @returns A new rainfall station
			/// @param Id An identifying number
			/// @param Name Name of the station
			/// @param Data Rainfall timeseries
			/// @param position Spatial position of the new station
			static ptr create(size_t Id,std::string Name, cmf::math::timeseries Data, cmf::geometry::point position);

			/// A numerical identifier
			const size_t id;
			/// The name of the station
			const std::string name;
			/// The timeseries data
			cmf::math::timeseries data;
			/// Returns the name and the mean yearly rainfall
			std::string tostring() const;
			/// Contains the solute cocentration data
			cmf::water::SoluteTimeseries concentration;
			/// Connects a cell with this rainfall station
			void use_for_cell(cmf::upslope::Cell& c);
			/// copy c'tor
			RainfallStation(const RainfallStation& copy);
		};
		
		/// @ingroup meteo
		/// A list of rainfall stations
		class RainfallStationList {
			typedef std::vector<RainfallStation::ptr> rfs_vector;
			rfs_vector m_stations;
		public:
			/// Returns the number of rainfall stations
			size_t size() const { return m_stations.size();}
			
			/// Returns the station at index
			RainfallStation::ptr operator[](int index);
			

			/// Returns the station at index
			const RainfallStation::ptr operator[](int index) const;
			
			
			/// Creates a new RainfallStation and adds it to the list. Usage:
			/// @code project.rainfall_stations.add("Name", cmf.timeseries.from_file('rainfall.bin'), (65124,78009,187)) @endcode
			/// The position of the rainfall station will be used as identifier
			/// @returns A new rainfall station
			/// @param Name Name of the station
			/// @param Data Rainfall timeseries
			/// @param Position Spatial position of the new station
			RainfallStation::ptr add(std::string Name, cmf::math::timeseries Data, cmf::geometry::point Position) {
				RainfallStation::ptr rfs=RainfallStation::create(size(),Name,Data,Position);
				m_stations.push_back(rfs);
				return rfs;
			}
			/// Removes the station at index from this list
			void remove(int index) {
				m_stations.erase(m_stations.begin() + (index > 0 ? index : int(size()) + index));
			}
		};
		
		/// @ingroup meteo boundary
		/// References a single RainfallStation to provide rainfall intensity data
		class RainfallStationReference : public RainSource {
			RainfallStation::ptr m_station;
			RainfallStationReference(const cmf::project& project, cmf::geometry::point position, RainfallStation::ptr station);
		public:
			/// shared_ptr
			typedef std::tr1::shared_ptr<cmf::atmosphere::RainfallStationReference > ptr;
			
			/// Finds the nearest RainfallStation to position using z_weight for cmf::geometry::point::z_weight_distance
			/// This method is used by cmf::project::use_nearest_rainfall. 
			/// @returns the nearest station to position
			/// @param project The actual project
			/// @param position The actual position
			/// @param z_weight The weight of height differences to find the "nearest" station
			static ptr from_nearest_station(const cmf::project& project, cmf::geometry::point position, double z_weight);
			
			/// Creates a RainfallStationReference for a certain rainfall station
			static ptr from_station_id(const cmf::project& project, cmf::geometry::point position, size_t id);
			
			real get_intensity(cmf::math::Time t) const {
				return m_station->data[t];
			}
			
			real conc(cmf::math::Time t, const cmf::water::solute& Solute) {
				return m_station->concentration.conc(t,Solute);
			}

		};

		/// @ingroup meteo boundary
		/// A RainSource using an spatially interpolated rainfall intensity from all stations. 
		/// Interpolation method is inverse distance weighted (IDW)
		class IDWRainfall : public RainSource {
			typedef std::map<RainfallStation::ptr,real> weight_map;
			weight_map m_weights;
			IDWRainfall(const cmf::project& project, cmf::geometry::point position, double z_weight, double power);
		public:
			/// Creates an IDWRainfall object. This method is invoked by cmf::project::use_IDW_rainfall.
			/// @sa IDW
			/// @returns The new object
			/// @param project The actual project
			/// @param position The actual position
			/// @param z_weight The weight of height differences for the IDW method
			/// @param power (double)
			static ptr create(const cmf::project& project, cmf::geometry::point position, double z_weight, double power);
			real get_intensity(cmf::math::Time t) const;
			real conc(cmf::math::Time t, const cmf::water::solute& Solute);


		};

	}
}
#endif // Precipitation_h__
