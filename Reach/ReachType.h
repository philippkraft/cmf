#ifndef ReachType_h__
#define ReachType_h__

namespace cmf {
	namespace river {
		class Channel;
		///@brief Structure for the description of structural parameters of a reach
		///Abstract base class for different channel geometries
		struct ReachType
		{
			double
				nManning; ///<Manning's n (roughness coefficient)

			/// @brief Calculates the flow width from a given actual depth [m] using the actual channel %geometry
			virtual double Width(double depth) const=0;
			/// @brief Calculates the wetted perimeter from a given actual depth [m] using the actual channel %geometry
			virtual double Perimeter(double depth) const=0;
			/// @brief Calculates the actual depth of the reach using the channel %geometry
			/// @returns  Depth of the reach [m]
			/// @param area Wetted area of a river cross section [m<sup>2</sup>], can be obtained by V/l, where V is the stored volume and l is the reach length
			virtual double Depth(double area) const=0;
			/// @brief Calculates the wetted area from a given depth using the channel %geometry. In most cases use Area=V/l, where V is the stored volume and l is the reach length
			/// @returns   Wetted area of a river cross section [m<sup>2</sup>]
			/// @param depth Depth of the reach [m]
			virtual double Area(double depth) const=0;

			/// Calculates the flow rate from a given water volume in the reach
			/// \f{eqnarray*}
			/// q_{Manning}&=& A R^{\frac 23} \sqrt{\frac {\Delta_z} n} \\
			/// A &=& \frac V l \mbox{ Area of the wetted crossection, Volume per length}\\
			/// R&=&\frac A P(d) \\
			/// P(d)&=& \mbox{ Perimeter of the wetted crossection, a function of reach depth} \\
			/// d(V)&=& \mbox{ Depth of the reach} \\
			/// \Delta_z&=& \frac{z_{max} - z_{min}}{l} \mbox{ Slope of the reach} \\
			/// \f}
			/// @returns  Flow rate [m<sup>3</sup>/s]
			/// @param A The area of the cross section [m<sup>2</sup>]
			/// @param slope The slope of the reach [m/m]
			double qManning(double A,double slope) const;
			ReachType() : nManning(0.035) {}
			virtual ReachType* copy() const=0;
			Channel* AsChannel(double length);
		};


		///@brief Structure for the description of structural parameters of a reach
		///
		///Uses the SWAT channel geometry (see SWAT Theoretical Documentation, Version 2005 (ch. 7:1.1), Neitsch et al. 2005), in this class referenced as SWATtheoDoc.
		///Differences to the SWAT geometry: The flood plain is not plain, but has a small slope=0.5%, but has an infinite width
		struct SWATReachType : public ReachType
		{
			double
				BottomWidth, ///<Width of the channel bottom \f$ w_{bottom} [m] \f$
				ChannelDepth, ///<Depth of the channel  \f$ d_{channel} [m] \f$
				BankSlope, ///<Inverse slope of the river bank \f$ \Delta_{bank} \left[\frac m m\right] \f$
				FloodPlainSlope; ///<Inverse slope of the flood plain \f$ \Delta_{flood\ plain} \left[\frac m m\right] \f$

			///@brief Calculates the flow width from a given actual depth [m] using the actual channel geometry
			/// \f{eqnarray*}
			/// \mbox{If }  d\le d_{channel} && \\
			/// w &=& w_{bottom} + 2  \Delta_{bank} d \\
			/// \mbox{else,         } && \mbox{if the river floods the flood plain} \\
			/// w &=& w_{bank} + 2 \Delta_{Floodplain} (d-d_{channel} \\
			/// \f}
			///@see SWATtheoDoc eq. 7:1.1.3
			virtual double Width(double depth) const;
			///@brief Calculates the wetted perimeter from a given actual depth [m] using the actual channel geometry
			/// \f{eqnarray*}
			/// \mbox{If } d\le d_{channel} && \\
			/// P &=& w_{bottom} + 2  \sqrt{1+ {\Delta_{bank}}^2} d \\
			/// \mbox{else,         } && \mbox{if the river floods the flood plain} \\
			/// P &=& P(d_{channel} + 2 \sqrt{1+ {\Delta_{flood\ plain}}^2} (d-d_{channel}) \\
			/// \f}
			///@see SWATtheoDoc eq. 7:1.1.5
			///@param depth Actual depth of the reach [m]
			virtual double Perimeter(double depth) const;
			///@brief Calculates the actual depth of the reach using the channel geometry
			/// \f{eqnarray*}
			/// d &=& \sqrt{\frac{A}{\Delta_{bank}} + \frac{{w_{bottom}}^2}{4 {\Delta_{bank}}^2}} - \frac{w_{bottom}}{2 \Delta_{bank}} \\
			/// \mbox{If } d>d_{channel} &&\\
			/// d&=&d_{channel}+\sqrt{\frac{A-A(d_{channel})}{\Delta_{flood\ plain}} + \frac{{w(d_{channel})}^2}{4 {\Delta_{flood\ plain}}^2}} - \frac{w(d_{channel})}{2 \Delta_{flood\ plain}} \\
			/// \f}
			///@see SWATtheoDoc eq. 7:1.2.4
			/// @returns  Depth of the reach [m]
			/// @param area Wetted area of a river cross section [m<sup>2</sup>], can be obtained by V/l, where V is the stored volume and l is the reach length
			virtual double Depth(double area) const;
			/// @brief Calculates the wetted area from a given depth using the channel geometry. In most cases use Area=V/l, where V is the stored volume and l is the reach length
			/// \f{eqnarray*}
			/// \mbox{If } d>d_{channel} &&\\
			/// A &=& \left(w_{bottom} + \Delta_{bank} d\right) d \\
			/// \mbox{else,         } && \mbox{if the river floods the flood plain} \\
			/// A &=& A(d_{channel}) + \left(w(d_{channel} + \Delta_{flood\ plain} \left(d-d_{channel}\right)\right) (d-d_{channel}) \\
			/// \f}
			/// @see SWATtheoDoc eq. 7:1.1.4
			/// @returns   Wetted area of a river cross section [m<sup>2</sup>]
			/// @param depth Depth of the reach [m]
			virtual double Area(double depth) const;

			///Creates a new reach structure with standard values (small natural river)
			///BottomWidth = 3m, ChannelDepth = 0.5m, BankSlope = 2, nManning = 0.0035, FloodPlainSlope = 200
			SWATReachType();
			///Creates a new reach structure from a give width and depth
			/// @param BankWidth Width of the reach from bank to bank [m]
			/// @param Depth Depth of the reach [m]
			SWATReachType(double BankWidth,double Depth);
			SWATReachType* copy() const { return new SWATReachType(BottomWidth+2*BankSlope*ChannelDepth,ChannelDepth);}
		};
		/// @brief Structure for the description of reaches with a triangular cross section
		///
		/// Although real triangular cross section reach are rarely met, a triangular reach does scale with its water load, and is therefore preferable in case where nothing about channel geometry is known
		struct TriangularReach : public ReachType
		{
			/// Inverse slope of the bank \f$ \Delta = 0.5 \frac w d \f$
			double BankSlope;
			/// Returns the width of the  stream at a given depth
			/// \f{eqnarray*}
			/// w &=& 2 \Delta\ d
			/// \f}
			virtual double Width(double depth) const;
			/// Returns the wetted perimeter at a given depth
			/// \f{eqnarray*}
			/// P &=& 2 d \sqrt{1+\Delta^2}
			/// \f}
			virtual double Perimeter(double depth) const;
			/// Returns the depth at a given crossection area
			/// \f{eqnarray*}
			/// d &=& \sqrt{\frac{A}{\Delta}}
			/// \f}
			virtual double Depth(double area) const;
			/// Returns the crossection area at a given depth
			/// \f{eqnarray*}
			/// A &=& d^2 \Delta
			/// \f}
			virtual double Area(double depth) const;
			/// Creates a new triangular reach type
			TriangularReach(double bankSlope=2);
			TriangularReach* copy() const { return new TriangularReach(BankSlope);}

		};
		/// Describes a channel with a rectangular crosssection
		struct RectangularReach : public ReachType
		{
		private:
			double m_width;
		public:
			/// Returns the width of the  stream at a given depth
			/// \f{eqnarray*}
			/// w &=& const
			/// \f}
			virtual double Width(double depth) const;
			/// Returns the wetted perimeter at a given depth
			/// \f{eqnarray*}
			/// P &=& 2 d + w
			/// \f}
			virtual double Perimeter(double depth) const;
			/// Returns the depth at a given crossection area
			/// \f{eqnarray*}
			/// d &=& \frac A w
			/// \f}
			virtual double Depth(double area) const;
			/// Returns the crossection area at a given depth
			/// \f{eqnarray*}
			/// A &=& d w
			/// \f}
			virtual double Area(double depth) const;
			/// Creates a new rectangular reach type with width [m]
			RectangularReach(double width) : ReachType(), m_width(width) {}
			RectangularReach* copy() const;

		};
		struct PipeReach : public ReachType
		{
		private:
			double m_Radius;
		public:
			/// \f[w=2\sqrt{\left|r^2-(r-d)^2\right|} \f]
			virtual double Width(double depth) const;
			/// \f[P=r\arccos{\frac{r-d}{r}} \f]
			virtual double Perimeter(double depth) const;
			/// \f[d=r\left(1-\cos{\frac{A}{r^2}}\right) \f]
			virtual double Depth(double area) const;
			/// \f[A=r^2\arccos{\frac{r-d}{r}{r}} \f]
			virtual double Area(double depth) const;
			/// Creates a tube channel with diameter [m]
			PipeReach(double diameter) :ReachType(), m_Radius(diameter * 0.5) {}
			PipeReach* copy() const;
		};

		/// Creates a reachtype using a short cut character.
		/// Acceptes one of the following characters:
		/// - 'T' TriangularReach, width and depth are ignored.
		/// - 'R' RectangularReach, depth is ignored
		/// - 'P' PipeReach, depth is ignored, width is the diameter of the pipe
		/// - 'S' SWATReachType, a trapezoid flow cross section, as used in the SWAT model, width (bank width) and depth are used
		/// @returns the reach type
		/// @param shortcut Describes the geometry of the reach cross section.
		/// @param width Width of the reach cross section in m (might be ignored)
		/// @param depth Depth of the reach (might be ignored)
		cmf::river::ReachType* create_reachtype(char shortcut,double width=1.,double depth=0.25);
	}
}
#endif // ReachType_h__
