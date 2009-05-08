#ifndef maps_h__
#define maps_h__
#include <map>
#include <vector>
#include "geometry.h"
#include "Raster.h"
namespace cmf {
	/// Distributing parameter classes in space
	///
	/// @image html "spatialdistribution.png"
	/// When a study area is set up, a lot of data is given as maps. To assign upslope cells
	/// at a given position with georeferenced input dataset the use of spatial distributors,
	/// called maps in cmf might be useful, in some cases (like the usage of upslope cell
	/// factories) needed.
	/// 
	/// On the C++ side, the implementation of the distributors is class independent (using
	/// templates), but on the Python side, the distributed class has to be predefined (in
	/// the C++ wrapper). For the following classes distributors are defined (so far):
	///  - Numbers (int,float,double)
	///  - SoilType
	///  - SoilProfile
	///  - Pointer to IVegetation (the generalisation of all vegetation types)
	///  - Pointer to Meteorology
	/// There is an abstract base class IMap<class> from which all classes derive.
	namespace maps {
		
		/// An interface, that provides a get(x,y) function, that returns the value or
		/// object at the given location (x,y) in planar coordinates, and a has(x,y) function
		/// the returns true, if an object exists at this location <b>The coordination system
		/// has to be the same as used for the UpslopeCells, since no conversion between coordinate
		/// systems is provided</b>
		/// In Python the name resolves to IMap_* where * is one of the following: Int, Float,
		/// Double, SoilType, SoilProfile, Vegetation, Meteorology. The same convention applies
		/// to the following derived classes.
		template<class _Ty>
		class IMap
		{
		protected:
			///Returns a Soiltype at a location (pure virtual)
			virtual _Ty _get(double x,double y) =0;
			virtual bool _has(double x,double y) {return true;}
		public:
			///Returns a soil type by calling the protected virtual function get
			_Ty get(cmf::geometry::point p) {return _get(p.x,p.y);}
			///Returns a soil type by calling the protected virtual function get
			_Ty get(double x,double y) {return _get(x,y);}
			bool has(double x,double y) {return _has(x,y);}
			bool has(cmf::geometry::point p) {return _has(p.x,p.y);}
			_Ty __call__(double x,double y) {return _get(x,y);}
#ifndef SWIG
			_Ty operator()(double x,double y) {return _get(x,y);}
			_Ty operator()(cmf::geometry::point p) {return get(p);}
#endif
		};
		/// A SingleValueMap is the simplest possible implementation of the IMap interface.
		/// the get(x,y) function returns always the same value/object no matter what location
		/// is given</p>
		/// @code
		/// import CMFLib as cmf
		/// number=1
		/// map=SingleValueMap_Int(number)
		/// print map.get(0,0) # Returns 1
		/// @endcode
		template<class _Ty>
		class SingleValueMap:public IMap<_Ty>
		{
		private:
			_Ty m_Value;
			virtual _Ty _get(double x,double y)	{		return m_Value;			}
		public:
			SingleValueMap(_Ty value):m_Value(value) {}
		};

		/// Distributes a scalar value (int, float or double) with a raster dataset. The simplest
		/// way to create a continuous raster map is to load a raster in the ESRI ASCII format.
		/// @code
		/// import CMFLib as cmf
		/// conversionfactor=0.01 # A factor applied to the raster for unit conversion, optional
		/// map=ContinuousRasterMap_float('dem10.dem',conversionfactor) # A floating point ASCII-Raster
		/// print map.get(0,0) # Returns the value at loacation 0,0 (might be -9999 if has(0,0)==false)
		/// @endcode
		template<class _Ty>
		class ContinuousRasterMap : public IMap<_Ty>
		{
		private:
			_Ty m_NoData;
		protected:
			virtual _Ty _get(double x,double y)
			{
				if (Raster.HasData(x,y))
					return Raster.IdentifyXY(x,y);
				else
					return m_NoData;
			}
			virtual bool _has(double x,double y)
			{
				return Raster.HasData(x,y);
			}
		public:

			/// Creates a new continuous map
			/// @param raster Pointer to an existing cmf::geometry::Raster
			ContinuousRasterMap(const cmf::geometry::Raster<_Ty>& raster) : Raster(raster) {
				m_NoData=Raster.NoData();
			}
			/// Creates a new continuous map
			/// @param ASCFileName A file name of an ESRI ASC raster dataset
			/// @param factor Scales the raster dataset (e.g. unit conversion)
			/// @param nodata Value returned, if the raster has no data (if has(x,y)=false) default: -9999
			ContinuousRasterMap(std::string ASCFileName,_Ty factor=1,_Ty nodata=-9999) : Raster(ASCFileName),m_NoData(nodata) {
				if (factor!=1)
				{
					Raster*=factor;
				}
			}
			cmf::geometry::Raster<_Ty> Raster;

		};
		
		/// Distributes a discrete value or object as a given in an integer raster dataset according
		/// to a lookup table. To use this, one have to provide the dstribution raster and add
		/// id object pairs to the map prior to use.
		/// @code
		/// import CMFLib as cmf
		/// map=DiscreteRasterMap_SoilType('soilmap.asc') # soilmap.asc holds the ASCII raster of the soilmap
		/// map.add(1,cmf.SoilType(15,0.4,0.3,0.2,0.1,4.2)) # A soil type is registered with raster value 1
		/// map.add(2,cmf.SoilType(5,0.6,0.2,0.5,0.3,8.2))  # Another soil type is registered with raster value 2
		/// print map.get(0,0).Phi # prints the porosity of the soil at position (0,0)
		/// @endcode
		template<class _Ty>
		class DiscreteRasterMap:public IMap<_Ty>
		{
		private:
			typedef std::map<int,_Ty> map;
			map m_Values;
			_Ty m_NoData;
			bool m_delete_raster;
		protected:
			virtual _Ty _get(double x,double y) 
			{	
				if (Raster.HasData(x,y)) 
				{
					int index=Raster.IdentifyXY(x,y);
					map::iterator found=m_Values.find(index);
					if (found!=m_Values.end())
						return (found->second);			
					else
						return m_NoData;
				}
				else return m_NoData;
			}
			virtual bool _has(double x,double y)
			{
				if (Raster.HasData(x,y)) 
				{
					int index=Raster.IdentifyXY(x,y);
					map::iterator found=m_Values.find(index);
					if (found!=m_Values.end())
						return true;			
					else
						return false;
				}
				else return false;
			}
		public:
			geometry::Raster<int> Raster;
			/// Adds a value to the lookup table
			void add(int index,_Ty value)
			{
				m_Values[index]=value;
			}
			/// Erases the entry at a position
			void erase(int index)
			{
				map::iterator it=m_Values.find(index);
				if (it!=m_Values.end())
					m_Values.erase(it);
				else
					throw std::out_of_range("Element does not exist in map");
			}
			/// Creates a new map of discrete values, for example soil types
			/// @param raster Pointer to an existing cmf::geometry::Raster<int>
			/// @param NoDataValue Value returned, if the raster has no data (has(x,y)=false)
			DiscreteRasterMap(geometry::Raster<int>* raster,_Ty NoDataValue=_Ty()) 
				: Raster(*raster),m_NoData(NoDataValue)
			{}
			/// Creates a new map of discrete values, for example soil types
			/// @param RasterFileName File name of an ESRI ASC-raster, containing the handles
			/// @param NoDataValue Value returned if the handle is raster.NoData() or the value is not found in the lookup table
			DiscreteRasterMap(std::string RasterFileName,_Ty NoDataValue=_Ty()) 
				: Raster(RasterFileName),m_NoData(NoDataValue)
			{				}

		};
		
		/// Registers objects with locations and returns the nearest neighbor to the queried
		/// location. Distance is measured as: \f$ d = \sqrt{(x1-x2)^2+(y1-y2)^2} + w_z|z1-z2| \f$
		/// with w<sub>z</sub> is the weight factor for height differences, zWeight. A high w<sub>z</sub> may be used for contured
		/// object distribution, where \f$ \sqrt{(x1-x2)^2+(y1-y2)^2} \f$ is small compared to \f$ w_z|z1-z2| \f$
		/// @code
		/// import CMFLib as cmf
		/// map=cmf.NearestNeighborMap_float()
		/// map.add(0,1,0.1) # Adds 0.1 at position (0,1)
		/// map.add(1,3,7.8) # Adds 7.8 at position (1,3)
		/// print map.get(0,0) # prints 0.1 (0,1) is nearer to (0,0) then (1,3)
		/// @endcode
		template <class _Ty> class NearestNeighborMap: public IMap<_Ty>
		{
		private: 
			typedef std::pair<cmf::geometry::point,_Ty> npair;
			typedef std::vector<npair> nearestNeighborVector;
			nearestNeighborVector m_Values;
		protected:
			virtual _Ty _get(double x,double y)
			{
				cmf::geometry::point p(x,y);
				double minDist=p.distanceTo(m_Values.begin()->first);
				_Ty bestValue=m_Values.begin()->second;
				for(nearestNeighborVector::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
				{
				   double dist=p.distanceTo(it->first)+zWeight*abs(p.z-it->first.z);
					 if (dist<minDist)
					 {
						 minDist=dist;
						 bestValue=it->second;
					 }
				}
				return bestValue;
			}
		public:
			/// Adds a value at a location
			/// @param p location of value
			/// @param value Value
			void add(cmf::geometry::point p,_Ty value)
			{
				m_Values.push_back(npair(p,value));
			}
			/// Adds a value at a location
			/// @param x X-Value of location
			/// @param y Y-Value of location
			/// @param value Value
			void add(double x,double y,_Ty value)
			{
				add(cmf::geometry::point(x,y),value);
			}
			/// Erases the entry at a position
			void erase(int pos)
			{
				if (pos<int(m_Values.size()))
					m_Values.erase(m_Values.begin()+pos);
				else
					throw std::out_of_range("Element does not exist in map");
			}
			/// Weight of the z difference
			double zWeight;
			/// Returns the number of point value pairs in this map
			int size() { return int(m_Values.size());}
			/// Returns the position of a value
			cmf::geometry::point& Position(int index) { return m_Values.at(index).first;}
			/// Returns one of the values
			_Ty Value(int index) { return m_Values.at(index).second;}
			NearestNeighborMap(double z_Weight=0) : zWeight(z_Weight) {}

		};
		
		/// Should return objects or values that are registered with a polygon, if the queried
		/// point is in the polygon. This is not tested yet, nor exists a really simple example.
		/// Happy Hacking!
		template <class _Ty> class PolygonMap: public IMap<_Ty>
		{
		private:
			typedef std::vector<cmf::geometry::Ring> polygons;
			typedef std::vector<_Ty> values;
			polygons m_polys;
			values m_values;
			_Ty m_NoData;
		protected:
			virtual _Ty _get(double x,double y)
			{
				cmf::geometry::point p(x,y);
				for (size_t i = 0; i < m_polys.size() ; ++i)
				{
					if (m_polys[i].Includes(p)) return m_values[i];
				}
				return m_NoData;
			}
		public:
			/// Creates a new map based on polygon/value pairs
			/// @param nodata Value returned, if no polygon is found at the given location
			PolygonMap(_Ty nodata=_Ty()):	m_NoData(nodata) {}
			/// Adds a ring with a value
			/// Makes a copy of polygon and stores it in the list of values
			void add(const cmf::geometry::Ring& polygon,_Ty value)
			{
				m_polys.push_back(polygon);
				m_values.push_back(value);
			}
			/// Erases the entry at a position
			void erase(int pos)
			{
				if (pos<int(m_values.size()))
					m_values.erase(m_values.begin()+pos);
				else
					throw std::out_of_range("Element does not exist in map");
			}
			/// Returns the number of point value pairs in this map
			int size() { return int(m_values.size());}
			/// Returns the polygon of a value
			cmf::geometry::Ring& Polygon(int index) { return m_polys.at(index);}
			/// Returns one of the values
			_Ty Value(int index) { return m_values.at(index);}

		};		
	}
}

#endif // maps_h__
