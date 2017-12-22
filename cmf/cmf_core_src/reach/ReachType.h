

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
#ifndef ReachType_h__
#define ReachType_h__
#include "../cmfmemory.h"

namespace cmf {
	namespace river {

		/// @brief Volume height relations are functional objects, which return a height and a crosssectional area of a volume for different geometric bodies.
		/// This is the abstract base class, where the geometries derive from
		class IVolumeHeightFunction
		{
		public:
			/// Returns the depth of a given volume
			virtual double h(double V) const=0;
			/// Returns the area of the surface for a given volume
			virtual double A(double V) const=0;
			virtual double V(double h) const=0;
			virtual IVolumeHeightFunction* copy() const=0;
			virtual double q(double h,double slope) const{
				return 0.0;
			}
			virtual ~IVolumeHeightFunction()
			{

			}
		};
		/// @brief the height of a volume in a Prism with a defined base area
		class Prism : public IVolumeHeightFunction
		{
		public:
			double Area;
			double RoughThickness;
			Prism(double base_area, double thickness_of_rough_ground=0.01);
			Prism* copy() const {return new Prism(Area);}
			/// \f$ h(V) = \frac V A_{base} \f$
			double h(double V) const {return V/Area;}
			/// \f$ A(V) = A_{base} \f$
			double A(double V) const;
			double V(double h) const {return h*Area;}
			~Prism()
			{

			}
		};
	    /// @brief A wrapper class for volume / height functional relations
		class volume_height_function : public IVolumeHeightFunction
		{
		private:
			std::unique_ptr<IVolumeHeightFunction> vhf;
		public:
			/// Copy constructable
			volume_height_function(const volume_height_function& for_copy)
				: vhf(for_copy.vhf->copy()) {}
			/// Wrapper for any IVolumeHeightFunction
			volume_height_function(const IVolumeHeightFunction& for_copy)
				: vhf(for_copy.copy()) {}
#ifndef SWIG
			// assignable
			volume_height_function& operator=(const volume_height_function& for_copy) {
				vhf.reset(for_copy.copy());
				return *this;
			}
			volume_height_function& operator=(const IVolumeHeightFunction& for_copy) {
				vhf.reset(for_copy.copy());
				return *this;
			}
#endif
			volume_height_function* copy()	const {
				return new volume_height_function(*this);
			}
			double h(double V) const {return vhf->h(V);}
			double A(double V) const {return vhf->A(V);}
			double V(double h) const {return vhf->V(h);}
			double q(double h,double slope) const {return vhf->q(h,slope);}
			virtual ~volume_height_function()
			{

			}
		};

		///@brief Structure for the description of structural parameters of a reach
		///Abstract base class for different IChannel geometries
		class IChannel : public cmf::river::IVolumeHeightFunction
		{
		public:
			virtual double get_nManning() const=0;
			virtual void set_nManning(double val)=0;

			virtual double get_length() const=0; ///< Length of the reach

			virtual char typecode() const=0;
			/// @brief Calculates the flow width from a given actual depth [m] using the actual IChannel %geometry
			virtual double get_channel_width(double depth) const=0;
			/// @brief Calculates the wetted perimeter from a given actual depth [m] using the actual IChannel %geometry
			virtual double get_wetted_perimeter(double depth) const=0;
			/// @brief Calculates the actual depth of the reach using the IChannel geometry
			/// @returns  get_depth of the reach [m]
			/// @param area Wetted area of a river cross section [m<sup>2</sup>], can be obtained by V/l, where V is the stored volume and l is the reach length
			virtual double get_depth(double area) const=0;
			
			/// @brief Calculates the wetted area from a given depth using the IChannel geometry. 
			/// In most cases use get_flux_crossection=V/l, where V is the stored volume and l is the reach length
			/// @returns Wetted area of a river cross section [m<sup>2</sup>]
			/// @param depth depth of the reach [m]
			virtual double get_flux_crossection(double depth) const=0;

			virtual double h(double V) const {return get_depth(V/get_length());}
			virtual double A(double V) const {return get_channel_width(h(V))*get_length();}
			virtual double V(double h) const {return get_flux_crossection(h)*get_length();}
			virtual double q(double h,double slope) const {return qManning(get_flux_crossection(h),slope);}
			virtual IChannel* copy() const=0;

			/// @brief Calculates the flow rate from a given water volume in the reach
			/// \f{eqnarray*}
			/// q_{Manning}&=& A R^{\frac 2 3} \sqrt{\frac {\Delta_z} n} \\
			/// A &=& \frac V l \mbox{, (Crosssectional area of the wetted crossection, Volume per length)} \\
			/// R &=& \frac A {P(d)} \\
			/// P(d) &=& \mbox{ the perimeter of the wetted crosssection, a function of reach depth} \\
			/// d(V) &=& \mbox{ the depth of the reach a function of the volume} \\
			/// \Delta_z &=& \frac{z_{max} - z_{min}}{l} \mbox{ Slope of the reach}
			/// \f}
			/// @returns  Flow rate [m<sup>3</sup>/s]
			/// @param A The area of the cross section [m<sup>2</sup>]
			/// @param slope The slope of the reach [m/m]
			virtual double qManning(double A,double slope) const;
			virtual ~IChannel()
			{

			}
		};


// 		/// Calculates flow on a rough surface
// 		///
// 		/// - d_puddle is the average depth of the water table when run off starts
// 		/// - d_rill is the average depth of rills (multiple triangular structure)
// 		/// 
// 		/// Structural behaviour:
// 		///
// 		/// \f[w(h) = \min(1,\frac{h}{h_0 + h_r})w_{max}\f]
// 		/// \f[A(h) = w(h) h\f]
// 		/// \f[q_{Manning} = A\frac{S^{1/2}}{n}(h-h_0)^{e_m}\f]
// 		///  - \f$q_{Manning}\f$ is the flow in m3/s
// 		///  - \f$S\f$ is the max. slope of the element
// 		///  - \f$n\f$ is Manning roughness
// 		///  - \f$h\f$ is the avg. depth of water above the ground
// 		///  - \f$h_0\f$ is the minimum avg. water depth to generate flow (PuddleDepth)
// 		///  - \f$e_m\f$ is the kinematic wave exponent. For surface flow it is typically 2/3 - 5/3
// 		class FlowSurface: public IChannel {
// 		protected:
// 			double m_width;
// 			double m_length;
// 			double m_nManning;
// 		public:
// 			double d_puddle;
// 			double d_rill;
// 			double e_m;
// 			virtual double get_nManning() const {return m_nManning;}
// 			virtual void set_nManning(double nManning) {m_nManning=nManning;}
// 			virtual double qManning(double A,double slope) const;
// 			virtual double get_length() const {return m_length;} ///< Length of the reach
// 			virtual char typecode() const {return 'A';}
// 			virtual double get_channel_width(double depth) const;
// 			/// @brief Calculates the wetted area from a given depth using the IChannel geometry. 
// 			/// In most cases use get_flux_crossection=V/l, where V is the stored volume and l is the reach length
// 			/// @returns Wetted area of a river cross section [m<sup>2</sup>]
// 			/// @param depth depth of the reach [m]
// 			virtual double get_flux_crossection(double depth) const {
// 				return get_channel_width(depth) * depth;
// 			}
// 			virtual double get_wetted_perimeter(double depth) const{
// 				return get_channel_width(depth);
// 			}
// 			virtual double get_depth(double area) const;
// 			FlowSurface(double length, double width,double d_puddle=0.0,double d_rill=0.0,
// 				double nManning=0.035, double e_m=0.6666667);
// 			FlowSurface(const FlowSurface& other);
// 			FlowSurface* copy() const {
// 				return new FlowSurface(*this);
// 			}
// 
// 		};
// 

		///@brief Structure for the description of structural parameters of a reach
		///
		///Uses the SWAT IChannel geometry (see SWAT Theoretical Documentation, Version 2005 (ch. 7:1.1), Neitsch et al. 2005), in this class referenced as SWATtheoDoc.
		///Differences to the SWAT geometry: The flood plain is not plain, but has a small slope=0.5%, but has an infinite width
		class SWATReachType : public IChannel
		{
		private:
			double m_l;	
			double m_nManning;
		public:
			double get_length() const {return m_l;}
			char typecode() const{return 'S';}

			double
				BottomWidth, ///<get_channel_width of the IChannel bottom \f$ w_{bottom} [m] \f$
				ChannelDepth, ///<get_depth of the IChannel  \f$ d_{IChannel} [m] \f$
				BankSlope, ///<Inverse slope of the river bank \f$ \Delta_{bank} \left[\frac m m\right] \f$
				FloodPlainSlope; ///<Inverse slope of the flood plain \f$ \Delta_{flood\ plain} \left[\frac m m\right] \f$

			virtual double get_nManning() const {return m_nManning;}
			virtual void set_nManning(double nManning) {m_nManning=nManning;}

			///@brief Calculates the flow width from a given actual depth [m] using the actual IChannel geometry
			/// \f{eqnarray*}
			/// \mbox{If }  d\le d_{IChannel} && \\
			/// w &=& w_{bottom} + 2  \Delta_{bank} d \\
			/// \mbox{else,         } && \mbox{if the river floods the flood plain} \\
			/// w &=& w_{bank} + 2 \Delta_{Floodplain} (d-d_{IChannel} \\
			/// \f}
			///@see SWATtheoDoc eq. 7:1.1.3
			virtual double get_channel_width(double depth) const;
			///@brief Calculates the wetted perimeter from a given actual depth [m] using the actual IChannel geometry
			/// \f{eqnarray*}
			/// \mbox{If } d\le d_{IChannel} && \\
			/// P &=& w_{bottom} + 2  \sqrt{1+ {\Delta_{bank}}^2} d \\
			/// \mbox{else,         } && \mbox{if the river floods the flood plain} \\
			/// P &=& P(d_{IChannel} + 2 \sqrt{1+ {\Delta_{flood\ plain}}^2} (d-d_{IChannel}) \\
			/// \f}
			///@see SWATtheoDoc eq. 7:1.1.5
			///@param depth Actual depth of the reach [m]
			virtual double get_wetted_perimeter(double depth) const;
			///@brief Calculates the actual depth of the reach using the IChannel geometry
			/// \f{eqnarray*}
			/// d &=& \sqrt{\frac{A}{\Delta_{bank}} + \frac{{w_{bottom}}^2}{4 {\Delta_{bank}}^2}} - \frac{w_{bottom}}{2 \Delta_{bank}} \\
			/// \mbox{If } d>d_{IChannel} &&\\
			/// d&=&d_{IChannel}+\sqrt{\frac{A-A(d_{IChannel})}{\Delta_{flood\ plain}} + \frac{{w(d_{IChannel})}^2}{4 {\Delta_{flood\ plain}}^2}} - \frac{w(d_{IChannel})}{2 \Delta_{flood\ plain}} \\
			/// \f}
			///@see SWATtheoDoc eq. 7:1.2.4
			/// @returns  get_depth of the reach [m]
			/// @param area Wetted area of a river cross section [m<sup>2</sup>], can be obtained by V/l, where V is the stored volume and l is the reach length
			virtual double get_depth(double area) const;
			/// @brief Calculates the wetted area from a given depth using the IChannel geometry. In most cases use get_flux_crossection=V/l, where V is the stored volume and l is the reach length
			/// \f{eqnarray*}
			/// \mbox{If } d>d_{IChannel} &&\\
			/// A &=& \left(w_{bottom} + \Delta_{bank} d\right) d \\
			/// \mbox{else,         } && \mbox{if the river floods the flood plain} \\
			/// A &=& A(d_{IChannel}) + \left(w(d_{IChannel} + \Delta_{flood\ plain} \left(d-d_{IChannel}\right)\right) (d-d_{IChannel}) \\
			/// \f}
			/// @see SWATtheoDoc eq. 7:1.1.4
			/// @returns   Wetted area of a river cross section [m<sup>2</sup>]
			/// @param depth Depth of the reach [m]
			virtual double get_flux_crossection(double depth) const;

			SWATReachType(const SWATReachType& copy);

			///@brief Creates a new reach structure with standard values (small natural river)
			///BottomWidth = 3m, ChannelDepth = 0.5m, BankSlope = 2, nManning = 0.0035, FloodPlainSlope = 200
			SWATReachType(double l);
			///Creates a new reach structure from a give width and depth
			/// @param l length of the channel [m]
			/// @param BankWidth get_channel_width of the reach from bank to bank [m]
			/// @param Depth Depth of the reach [m]
			SWATReachType(double l,double BankWidth,double Depth);
			SWATReachType* copy() const;
			virtual ~SWATReachType()
			{

			}

		};
		/// @brief Structure for the description of reaches with a triangular cross section
		///
		/// Although double triangular cross section reach are rarely met, a triangular reach does scale with its water load, and is therefore preferable in case where nothing about IChannel geometry is known
		class TriangularReach : public IChannel
		{
		private:
			double m_l;	
			double m_nManning;
		public:
			virtual double get_nManning() const {return m_nManning;}
			virtual void set_nManning(double nManning) {m_nManning=nManning;}

			double get_length() const {
				return m_l;
			}
			char typecode() const{return 'T';}
			/// @brief Inverse slope of the bank \f$ \Delta = 0.5 \frac w d \f$
			double BankSlope;
			/// @brief Returns the width of the  stream at a given depth
			/// \f{eqnarray*}
			/// w &=& 2 \Delta\ d
			/// \f}
			virtual double get_channel_width(double depth) const;
			/// @brief Returns the wetted perimeter at a given depth
			/// \f{eqnarray*}
			/// P &=& 2 d \sqrt{1+\Delta^2}
			/// \f}
			virtual double get_wetted_perimeter(double depth) const;
			/// @brief Returns the depth at a given crossection area
			/// \f{eqnarray*}
			/// d &=& \sqrt{\frac{A}{\Delta}}
			/// \f}
			virtual double get_depth(double area) const;
			/// Returns the crossection area at a given depth
			/// \f{eqnarray*}
			/// A &=& d^2 \Delta
			/// \f}
			virtual double get_flux_crossection(double depth) const;
			/// @brief Creates a new triangular reach type
			TriangularReach(double l,double bankSlope=2);
			TriangularReach(const TriangularReach& copy);
			TriangularReach* copy() const;
			virtual ~TriangularReach()
			{

			}

		};
		/// @brief Describes a IChannel with a rectangular crosssection
		class RectangularReach : public IChannel
		{
		private:
			double m_width;
			double m_l;	
			double m_nManning;
		public:
			virtual double get_nManning() const {return m_nManning;}
			virtual void set_nManning(double nManning) {m_nManning=nManning;}
			double get_length() const {return m_l;}
			char typecode() const{return 'R';}
			/// Returns the width of the  stream at a given depth
			/// \f{eqnarray*}
			/// w &=& const
			/// \f}
			virtual double get_channel_width(double depth) const;
			/// Returns the wetted perimeter at a given depth
			/// \f{eqnarray*}
			/// P &=& 2 d + w
			/// \f}
			virtual double get_wetted_perimeter(double depth) const;
			/// Returns the depth at a given crossection area
			/// \f{eqnarray*}
			/// d &=& \frac A w
			/// \f}
			virtual double get_depth(double area) const;
			/// Returns the crossection area at a given depth
			/// \f{eqnarray*}
			/// A &=& d w
			/// \f}
			virtual double get_flux_crossection(double depth) const;
			/// Creates a new rectangular reach type with width [m]
			RectangularReach(double l,double width);
			RectangularReach(const RectangularReach& copy);
			RectangularReach* copy() const;
			virtual ~RectangularReach()
			{

			}
		};
		/// @brief Describes the geometry of a closed pipe
		class PipeReach : public IChannel
		{
		private:
			double m_l;	
			double m_nManning;
		public:
			virtual double get_nManning() const {return m_nManning;}
			virtual void set_nManning(double nManning) {m_nManning=nManning;}
			double get_length() const {return m_l;}
			char typecode() const{return 'P';}
			double radius;
			/// \f[w=2\sqrt{\left|r^2-(r-d)^2\right|} \f]
			virtual double get_channel_width(double depth) const;
			/// \f[P=r\arccos{\frac{r-d}{r}} \f]
			virtual double get_wetted_perimeter(double depth) const;
			/// \f[d=r\left(1-\cos{\frac{A}{r^2}}\right) \f]
			virtual double get_depth(double area) const;
			/// \f[A=r^2\arccos{\frac{r-d}{r}{r}} \f]
			virtual double get_flux_crossection(double depth) const;
			/// Creates a tube IChannel with diameter [m]
			PipeReach(double l,double diameter);
			PipeReach(const PipeReach& copy);
			PipeReach* copy() const;
			virtual ~PipeReach()
			{

			}
		};

		/// @brief A wrapper for channel geometries
		class Channel : public IChannel
		{
		private:
			std::unique_ptr<IChannel> m_channel;
		public:
			double get_length() const { return m_channel->get_length();}
			Channel();
			/// Creates a triangular reach of a length
			Channel(double length);
			/// Wraps an existing channel geometry
			Channel(const IChannel& for_wrapping);

			Channel(const IVolumeHeightFunction& for_casting);
			/// Copy constructable
			Channel(const Channel& for_copy);
#ifndef SWIG
			/// Assignable
			Channel& operator=(const Channel& for_assignment);
			Channel& operator=(const IChannel& for_assignment);
#endif
			
			/// @brief Creates a reachtype using a short cut character.
			/// Acceptes one of the following characters:
			/// - 'T' TriangularReach, width and depth are ignored.
			/// - 'R' RectangularReach, depth is ignored
			/// - 'P' PipeReach, depth is ignored, width is the diameter of the pipe
			/// - 'S' SWATReachType, a trapezoid flow cross section, as used in the SWAT model, width (bank width) and depth are used
			/// @returns the reach type
			/// @param typecode Describes the geometry of the reach cross section.
			/// @param length The length of the channel in m
			/// @param width width of the reach cross section in m (ignored for typecode 'T')
			/// @param depth depth of the reach (ignored for typecode 'T','R','P','S')
			Channel(char typecode, double length, double width=1.,double depth=0.25);
			
			char typecode() const {
				return m_channel->typecode();             
			}
			double get_channel_width(double depth) const {
				return m_channel->get_channel_width(depth);
			}
			double get_depth(double area) const	{
				return m_channel->get_depth(area); 
			}
			double get_flux_crossection(double depth) const	{
				return m_channel->get_flux_crossection(depth); 
			}
			double get_wetted_perimeter(double depth) const	{
				return m_channel->get_wetted_perimeter(depth); 
			}
			Channel* copy()	const;
			
			void set_nManning(double val) {m_channel->set_nManning(val);} 
			double get_nManning() const {return m_channel->get_nManning();}
			virtual ~Channel()
			{

			}
		};

		/// @brief A combination of two channel geometries
		class MeanChannel : public IChannel
		{
		private:
			Channel m_channel1,m_channel2;
		public:
			double get_length() const;
			/// Creates the mean geometry from the two channel geometries
			MeanChannel(const IChannel& channel1,const IChannel& channel2);
			MeanChannel(const MeanChannel& meanChannel);
			
			char typecode()  const;
			double get_channel_width(double depth)	const;
			double get_depth(double area)  const;
			double get_flux_crossection(double depth) const;
			double get_wetted_perimeter(double depth) const;
			virtual double get_nManning() const {return .5*(m_channel1.get_nManning()+m_channel2.get_nManning());}
			virtual void set_nManning(double nManning);

			MeanChannel* copy() const;
			virtual ~MeanChannel()
			{

			}

		};
		/*
		///@brief A channel with a piecewise linear shape
		///
		/// The geometry of this reach is defined by a discrete
		/// depth/width pairs with linear interpolation between the depth and width
		///
		/// \f$ i \f$ is the highest depth/width pair below a given depth \f$d\f$
		/// \f[w(d,i) = w_i + (d-d_i) \frac{w_{i+1}-w_i}{d_{i+1} - d_i}\f]
		/// \f[ A(d,i) =A(d_i,i-1)+ \frac12 (d-d_i)(w(d)+w_i)  \f]
		/// \f[P(d,i) = P(d_i,i-1) + 2\sqrt{\left(\frac{w-w_i}{2}\right)^2+\left(d-d_i\right)^2} \f]
		/// \f[P(0,0)=w_0 \f]
		class PiecewiseReach: public IChannel
		{

		};
		*/
	}
}
#endif // ReachType_h__
