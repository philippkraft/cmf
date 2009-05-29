#ifndef boundary_condition_h__
#define boundary_condition_h__
#include "FluxConnection.h"
#include "../math/timeseries.h"
namespace cmf {
	namespace water {
		class linear_scale
		{
		public:
			real displacement;
			real slope;
			real operator()(real value)	const
			{
				return value*slope+displacement;
			}
			linear_scale(real _slope=1,real _displacement=0)
				: slope(_slope),displacement(_displacement)
			{

			}
		};
		class DricheletBoundary : public cmf::water::FluxNode
		{
		protected:
			real m_Potential;
		public:
			real Potential() const
			{
				return m_Potential;
			}
			bool is_source;
			bool Empty() const
			{
				return !is_source;
			}
			bool RecalcFluxes(cmf::math::Time t) const
			{
				return 1;
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
			real operator()(cmf::math::Time t) const
			{
				if (flux.isempty())
					return 0.0;
				else
					return scale_function(flux[t]);
			}
			bool Empty() const
			{
				return flux.isempty();
			}
			
			/// Ctor of the Neumann boundary
			/// @param _project The project this boundary condition belongs to
			/// @param _flux The flux timeseries (a scalar is converted to a timeseries automatically)
			/// @param _concentration The concentration timeseries
			/// @param loc The location of the boundary condition
			NeumannBoundary(const cmf::project& _project, cmf::math::timeseries _flux=0.0,cmf::water::SoluteTimeseries _concentration=cmf::water::SoluteTimeseries(),cmf::geometry::point loc=cmf::geometry::point()) 
				: cmf::water::FluxNode(_project,loc), flux(flux),concentration(_concentration) {}
		};
		
			class NeumannFlux : public cmf::water::FluxConnection
		{
		protected:
			NeumannBoundary* m_bc;
			void NewNodes()
			{
				m_bc=dynamic_cast<NeumannBoundary*> (m_left);
			}
			real calc_q(cmf::math::Time t)
			{
				real f=(*m_bc)(t);
				if (f<0 && m_right->Empty())
					return 0.0;
				else
					return f;
			}
		public:
			
		};

		
	}
	
}
#endif // boundary_condition_h__
