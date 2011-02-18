#ifndef simple_connections_h__
#define simple_connections_h__


#include "flux_connection.h"
#include "flux_node.h"
#include "waterstorage.h"

namespace cmf {
	namespace water {
		/// @ingroup connections
		/// Routes the sum of all other fluxes to a target
		class waterbalance_connection : public flux_connection
		{
		protected:
			real calc_q(cmf::math::Time t) 
			{
				real q = left_node()->waterbalance(t,this);
				if (q > 0) return q;
				else return 0.0;
			}
			void NewNodes() {}
		public:
			waterbalance_connection(flux_node::ptr source,flux_node::ptr target) 
				: flux_connection(source,target,"waterbalance connection")
			{
				RecalcAlways=true;
			}
		};

		/// @ingroup connections
		/// Flux from one node to another, controlled by the user or an external program, 
		/// by changing the flux constant
		class external_control_connection : public flux_connection {
		protected:
			real calc_q(cmf::math::Time t)	{
				return flux;
			}
			void NewNodes() {}
		public:
			real flux;
			external_control_connection(flux_node::ptr source,flux_node::ptr target,real flux_value=0) 
				: flux_connection(source,target,"external controlled connection"),flux(flux_value)			
			{
				RecalcAlways=true;
			}
		};
		/// @ingroup connections
		/// Sets a constant flux between two nodes, if an external_control_connection exists. 
		// Otherwise an error is thrown
		void set_flux(flux_node::ptr source,flux_node::ptr target,real flux_value);

		/// @ingroup connections
		/// Checks if a constant flux between two nodes can be set. Returns true
		/// if the nodes are connected by an external_control_connection
		bool can_set_flux(flux_node::ptr source,flux_node::ptr target);

		/// @ingroup connections
		/// Calculates flux out of a storage as a linear function of its volume to a power.
		///
		/// \f[ q = \frac {V_{mobile}^\beta}{t_r} \f]
		/// where:
		/// - \f$V_{mobile} [m^3] = V - V_{residual}\f$ the stored mobile volume
		/// - \f$\beta [-]\f$ An empirical exponent to shape the flux function
		/// - \f$t_r [days]\f$ The residence time of the water in this storage in days
		class kinematic_wave : public flux_connection {
		protected:
			WaterStorage::ptr source;
			real calc_q(cmf::math::Time t);
			void NewNodes() {
				source = WaterStorage::cast(left_node());
			}
		public:
			/// Linear flow parameter traveltime in days
			real residencetime;
			/// Exponent of volume, beta
			real exponent;
			/// residual volume in m3
			real residual_volume;

			/// Creates a kinematic wave connection.
			/// \f[ q = \frac {\left(V - V_{residual}\right)^\beta}{t_r} \f]
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param residencetime \f$t_r [days]\f$ The residence time of the water in this storage
			/// @param exponent \f$\beta [-]\f$ An empirical exponent to shape the flux function (default = 1 (linear function))
			/// @param residual_volume \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0 m3)
			kinematic_wave(WaterStorage::ptr source,flux_node::ptr target,real residencetime, real exponent=1.0, real residual_volume=0.0);
		};

		/// @ingroup connections
		/// Produces a constant but changeable flux from a source to a target, if enough water is present in the source
		///
		/// \f$ q=\left\{0 \mbox{ if }V_{source}\le V_{min}\\ \frac{V_{source} - V_{min}}{t_{decr} q_{0} - V_{min}}\mbox{ if } V_{source} t_{decr} q_{0}\\ q_{0} \mbox{ else}\le \right. \f$
		class TechnicalFlux : public cmf::water::flux_connection
		{
		protected:
			std::tr1::weak_ptr<cmf::water::WaterStorage> source;
			virtual real calc_q(cmf::math::Time t)
			{
				return piecewise_linear(source.lock()->get_state(),MinState,MinState+FluxDecreaseTime.AsDays()*MaxFlux,0,MaxFlux);
			}
			void NewNodes()
			{
				source = cmf::water::WaterStorage::cast(left_node());
			}

		public:
			/// The requested flux \f$q_{0}\left[frac{m^3}{day}\right]\f$
			real MaxFlux;
			/// The minimal volume of the state  \f$V_{min}\left[m^3\right]\f$
			real MinState;
			/// The flux is linearly decreased, if it takes less than FluxDecreaseTime \f$t_{decr}\f$ to reach MinState with MaxFlux
			cmf::math::Time FluxDecreaseTime;

			/// Produces a constant but changeable flux from a source to a target, if enough water is present in the source
			/// @param source The source of the water
			/// @param target The target of the water
			/// @param maximum_flux The requested flux \f$q_{0}\f$
			/// @param minimal_state Minimal volume of stored water in source
			/// @param flux_decrease_time (cmf::math::Time)
			TechnicalFlux(std::tr1::shared_ptr<cmf::water::WaterStorage> & source,std::tr1::shared_ptr<cmf::water::flux_node> target,real maximum_flux,real minimal_state=0,cmf::math::Time flux_decrease_time=cmf::math::h)
				: flux_connection(source,target,"Technical flux"),MaxFlux(maximum_flux),MinState(minimal_state),FluxDecreaseTime(flux_decrease_time) {}
		};




	}
	
}
#endif // simple_connections_h__
