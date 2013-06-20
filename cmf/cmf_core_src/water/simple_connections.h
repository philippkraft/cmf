#ifndef simple_connections_h__
#define simple_connections_h__


#include "flux_connection.h"
#include "flux_node.h"
#include "WaterStorage.h"

namespace cmf {
	namespace water {
		/// @ingroup connections
		/// @brief Routes the sum of all other fluxes to a target
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
		/// @brief Flux from one node to another, controlled by the user or an external program, 
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
		/// @brief Sets a constant flux between two nodes, if an external_control_connection exists. 
		// Otherwise an error is thrown
		void set_flux(flux_node::ptr source,flux_node::ptr target,real flux_value);

		/// @ingroup connections
		/// @brief Checks if a constant flux between two nodes can be set. 
        ///
        /// Returns true if the nodes are connected by an external_control_connection
		bool can_set_flux(flux_node::ptr source,flux_node::ptr target);

		/// @ingroup connections
		/// @brief Calculates flux out of a storage as a linear function of its volume to a power.
		///
		/// \f[ q = \frac 1 {t_r} {\left(\frac{V}{V_0} - f_{residual}\right)^\beta} \f]
		/// where:
		/// - \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
		/// - \f$V_0\f$ The reference volume to scale the exponent
		/// - \f$t_r [days]\f$ The residence time of the water in this storage in days
		class kinematic_wave : public flux_connection {
		protected:
			WaterStorage::ptr source;
			real calc_q(cmf::math::Time t);
			void NewNodes() {
				source = WaterStorage::cast(left_node());
			}
		public:
			/// @brief Linear flow parameter traveltime in days
			real residencetime;
			/// @brief Exponent of volume, \f$\beta\f$
			real exponent;
			/// @brief residual fraction \f$f_r\f$ of reference volume V0
			real residual;
			/// @brief Reference volume \f$V_0\f$
			real V0;

			/// @brief Creates a kinematic wave connection.
			/// \f[ q = \frac {\left(\frac{V}{V_0} - f_{residual}\right)^\beta}{t_r} \f]
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param residencetime \f$t_r [days]\f$ The residence time of the water in this storage
			/// @param exponent \f$\beta [-]\f$ An empirical exponent to shape the flux function (default = 1 (linear function))
			/// @param residual \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
			/// @param V0 \f$V_0\f$ The reference volume to scale the exponent
			kinematic_wave(WaterStorage::ptr source,flux_node::ptr target,real residencetime,
							real exponent=1.0,real residual=0.0,real V0 = 1.0);
		};

		/// @ingroup connections
		/// @brief Produces a constant but changeable flux from a source to a target, if enough water is present in the source
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
			/// @brief The requested flux \f$q_{0}\left[frac{m^3}{day}\right]\f$
			real MaxFlux;
			/// @brief The minimal volume of the state  \f$V_{min}\left[m^3\right]\f$
			real MinState;
			/// @brief The flux is linearly decreased, if it takes less than FluxDecreaseTime \f$t_{decr}\f$ to reach MinState with MaxFlux
			cmf::math::Time FluxDecreaseTime;

			/// @brief Produces a constant but changeable flux from a source to a target, if enough water is present in the source
			///
            /// @param source The source of the water
			/// @param target The target of the water
			/// @param maximum_flux The requested flux \f$q_{0}\f$
			/// @param minimal_state Minimal volume of stored water in source
			/// @param flux_decrease_time (cmf::math::Time)
			TechnicalFlux(cmf::water::WaterStorage::ptr source,cmf::water::flux_node::ptr target,real maximum_flux,real minimal_state=0,cmf::math::Time flux_decrease_time=cmf::math::h)
				: flux_connection(source,target,"Technical flux"),MaxFlux(maximum_flux),MinState(minimal_state),FluxDecreaseTime(flux_decrease_time) {
					NewNodes();
			}
		};

		/// @brief A generic node-to-node gradient based connection. 
		
		/// This connection is similar to the Darcy-connection,
		/// but there are no restrictions concerning the type of nodes. However, one side should be a water storage.h
		/// \f[
		/// q = K A \frac{\Psi_{l}-\Psi_{r}}{d}
		/// \f]
		/// where:
		/// - \f$q\f$: the resulting flux in \f$m^3/day\f$
		/// - \f$K\f$: the conductivity of the connection
		/// - \f$A\f$: the area of the connection cross section
		/// - \f$\Psi\f$: The hydraulic head of the (l)eft, resp. (r)ight node of the connection
		/// - \f$d\f$: The topographic length of the connection in m
		class generic_gradient_connection
			: public flux_connection
		{
		protected:
			virtual real calc_q(cmf::math::Time t);
			virtual void NewNodes() {}
		public:
			/// Conductivity of the connection in m/day
			real K;
			/// Area of the connection cross section in m2
			real A;
			/// distance of the gradient
			real d;
			/// @brief Creates a generic gradient based flux, if enough water is present in the source
			///
			/// @param left The left node of the connection
			/// @param right The right node of the connection
			/// @param K the conductivity of the connection in m/day
			/// @param d the topographic lenght of the connection in m
			/// @param A the area of the connection cross section in m2
			generic_gradient_connection(cmf::water::WaterStorage::ptr left,cmf::water::WaterStorage::ptr right, real K,real d=1.0, real A=1.0); 
		};

		/// @ingroup connections
		/// @brief Calculates a flux to or from a water storage to hold it's state at a more or less constant level
		///
        /// \f[ q=\frac{h_1 - h_{target}}{t_c [days]} \f]
		class statecontrol_connection 
			: public flux_connection 
		{
		protected:
			std::tr1::weak_ptr<cmf::water::WaterStorage> source;
			virtual real calc_q(cmf::math::Time t);
			void NewNodes();
		public:
			cmf::math::Time reaction_time;
			real target_state;
			/// @brief Creates a flux connection to control the state of a storage
			/// @param controlled_storage Water storage, to be controlled
			/// @param other_end source of missing water or target of excessive water
			/// @param target_state State the controlled storage should hold (\f$h_{target}\f$)
			/// @param reaction_time Time to reach state (\f$t_c\f$)
			statecontrol_connection(cmf::water::WaterStorage::ptr controlled_storage, cmf::water::flux_node::ptr other_end, real target_state, cmf::math::Time reaction_time );
		};




	}
	
}
#endif // simple_connections_h__
