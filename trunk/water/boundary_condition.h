#ifndef boundary_condition_h__
#define boundary_condition_h__
#include "FluxConnection.h"
#include "../math/timeseries.h"
namespace cmf {
	namespace water {

		/// A linear scaling functor, with slope and displacement
		class linear_scale
		{
		public:
			/// Displacement of the scale ($b$ in scale function)
			real displacement;
			/// Slope of the scale ($a$ in scale function)
			real slope;
			/// Returns the scaled value $y_{scale}=a\ y+b$
			real operator()(real value)	const
			{
				return value*slope+displacement;
			}
			/// Creates a linear scale (by default it is a unity scale, $a=1; b=0$)
			linear_scale(real _slope=1,real _displacement=0)
				: slope(_slope),displacement(_displacement)
			{

			}
		};

		/// Drichelet (constant head) boundary condition
		///
		/// This boundary condition can be used either as a pure sink boundary condition or as a conditional source / sink boundary condition.
		/// The constant head of the boundary condition is interpreted and handled by the connections of the boundary condition. 
		/// Not head aware connections, should not be used, since they are ignoring the constant head.

		class DricheletBoundary : public cmf::water::FluxNode
		{
		protected:
			real m_Potential;
		public:
			real get_potential() const
			{
				return m_Potential;
			}
			void set_potential(real new_potential)
			{
				m_Potential=new_potential;
			}
			bool is_source;
			bool is_empty() const
			{
				return !is_source;
			}
			bool RecalcFluxes(cmf::math::Time t) const
			{
				return 1;
			}
			DricheletBoundary(const cmf::project& _p,real potential,cmf::geometry::point Location=cmf::geometry::point())
				: FluxNode(_p,Location),m_Potential(potential)
			{

			}
		};
		/// A Neumann boundary condition (constant flux boundary condition)
		///
		/// The flux is a timeseries, but can be used as a scalar.
		/// To scale the timeseries to the specific conditions of this boundary condition the linear_scale flux_scale can be used.
		class NeumannBoundary : public cmf::water::FluxNode
		{
		protected:
			virtual real scale_function(real value)	const
			{
				return flux_scale(value);
			}
		public:
			/// The timeseries of the boundary flux
			cmf::math::timeseries flux;
			/// The scaling of the flux timeseries
			linear_scale flux_scale;
			/// The concentration timeseries of the flux
			cmf::water::SoluteTimeseries concentration;
			/// Returns the solute concentrations of the flux at a given time
			WaterQuality conc(cmf::math::Time t) const
			{
				return concentration.conc(t);
			}
			/// Returns the flux at a given time
			real operator()(cmf::math::Time t) const;
			bool is_empty() const
			{
				return flux.is_empty();
			}
			bool RecalcFluxes(cmf::math::Time t) 
			{
				return true;
			}
			void connect_to(cmf::water::FluxNode& target);
			
			/// Ctor of the Neumann boundary
			/// @param _project The project this boundary condition belongs to
			/// @param _flux The flux timeseries (a scalar is converted to a timeseries automatically)
			/// @param _concentration The concentration timeseries
			/// @param loc The location of the boundary condition
			NeumannBoundary(const cmf::project& _project, cmf::math::timeseries _flux,cmf::water::SoluteTimeseries _concentration=cmf::water::SoluteTimeseries(),cmf::geometry::point loc=cmf::geometry::point()) 
				: cmf::water::FluxNode(_project,loc), flux(_flux),concentration(_concentration)
			{

			}
			NeumannBoundary(const cmf::project& _project,cmf::geometry::point loc=cmf::geometry::point()) 
				: cmf::water::FluxNode(_project,loc)
			{

			}

		};
		
		class NeumannFlux : public cmf::water::FluxConnection
		{
		protected:
			NeumannBoundary* m_bc;
			void NewNodes()
			{
				m_bc=dynamic_cast<NeumannBoundary*> (m_left);
			}
			real calc_q(cmf::math::Time t);
		public:
			NeumannFlux(NeumannBoundary& left,FluxNode& right)
				: FluxConnection(left,right,"Neumann boundary flux")
			{
				NewNodes();
			}
			
		};

		class NeumannBoundary_list
		{
		private:
			typedef std::vector<NeumannBoundary*> vector;
			vector m_boundaries;
		public:
#ifndef SWIG
			NeumannBoundary& operator[](int index) const
			{ return *m_boundaries.at(index<0 ? m_boundaries.size()+index : index );	}
#endif
			NeumannBoundary* get(int index) const
			{ return m_boundaries.at(index<0 ? m_boundaries.size()+index : index );	}
			real get_flux(int index,cmf::math::Time t) const {return (*get(index))(t);}
			void set_flux(int index,real value) {get(index)->flux=value;}
			cmf::math::numVector get_flux(cmf::math::Time t) const;
			void set_flux(cmf::math::numVector values);
			void append(NeumannBoundary* nbc)
			{
				m_boundaries.push_back(nbc);
			}
			int size() const
			{
				return m_boundaries.size();
			}
			NeumannBoundary_list() {};
			NeumannBoundary_list(const cmf::water::node_list& copy)
			{
				for (int i = 0; i < copy.size() ; ++i)
				{
					NeumannBoundary* nbc=dynamic_cast<NeumannBoundary*>(copy[i]);
					if (nbc)
						m_boundaries.push_back(nbc);
				}
			}
			NeumannBoundary_list(const NeumannBoundary_list& copy)
				: m_boundaries(copy.m_boundaries.begin(),copy.m_boundaries.end())
			{			}
			cmf::water::node_list to_node_list() const
			{
				cmf::water::node_list res;
				for (int i = 0; i < size() ; ++i)
				{
					res.append(get(i));
				}
				return res;
			}
			
			/// Returns the sum of the water balances of the nodes
			/// \f[\sigma_{global} = \sum_{i=0}^N{\sum_{j=0}^{C_i}{q_{ij}(t)}} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// sum=0
			/// for n in nodes:
			///     sum+=n.water_balance(t)
			/// @endcode
			real global_water_balance(cmf::math::Time t) const;
			/// Returns the water balance of each vector as a vector
			/// \f[ \sigma_i = sum_{j=0}^{C_i}{q_{ij}(t)}} \f]
			///
			/// Replaces slow Python code like:
			/// @code
			/// balances=[n.water_balance(t) for n in nodes]
			/// @endcode
			cmf::math::numVector water_balance(cmf::math::Time t) const;


		};

		
	}
	
}
#endif // boundary_condition_h__
