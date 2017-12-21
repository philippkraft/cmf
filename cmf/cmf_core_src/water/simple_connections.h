#ifndef simple_connections_h__
#define simple_connections_h__

#include "../math/real.h"
#include "flux_connection.h"
#include "flux_node.h"
#include "WaterStorage.h"

namespace cmf {
	namespace water {
		/// @ingroup connections
		/// @brief Routes the sum of all other fluxes to a target
		///
		/// \f[ q_{1,0} = \sum_{i=2}^N{q_{1,i}(V_1,V_i,t)}\f]
		/// where:
		/// - \f$q_{i,j}\f$ is the flux between the two node i and j. Subscript 0 is the right node,
		/// subscript 1 is the left node and 2..N are the nodes connected to the left node, except for the right node
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
			waterbalance_connection(flux_node::ptr source, flux_node::ptr target);
		};

		/// @ingroup connections
		/// @brief Flux from one node to another, controlled by the user or an external program, 
		/// by changing the flux constant
		///
		/// @note It is easy to create negative volumes in water storages with this connection, 
		/// which can be hazard to the solver, since most connections rely on a positive volume in a storage.
		/// Handle with care!
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
		void set_flux(flux_node::ptr source,flux_node::ptr target, real flux_value);

		/// @ingroup connections
		/// @brief Checks if a constant flux between two nodes can be set. 
        ///
        /// Returns true if the nodes are connected by an external_control_connection
		bool can_set_flux(flux_node::ptr source,flux_node::ptr target);

		/// @ingroup connections
		/// @brief Calculates flux out of a storage as a linear function of its volume to a power.
		/// @deprecated kinematic_wave is superseeded by LinearStorageConnection and PowerLawConnection
		///
		/// \f[ q = \frac 1 {t_r} {\left(\frac{V - V_{residual}}{V_0} \right)^\beta} \f]
		/// where:
		/// - \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
		/// - \f$V_0\f$ The reference volume to scale the exponent (default = 1m3/day)
		/// - \f$\beta\f$ A parameter to shape the response curve. In case of \f$\beta \neq 1\f$, 
		///   \f$t_r\f$ is not a residence time, but just a parameter.
		/// - \f$t_r [days]\f$ The residence time of the water in this storage in days
		///
		/// @warning \f$\beta < 0.5\f$ may lead to numerical troubles and have a dubious hydrological meaning. Please avoid.
		class kinematic_wave : public flux_connection {
		protected:
			std::weak_ptr<WaterStorage> source;
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
			/// \f[ q = \frac 1 {t_r} {\left(\frac{V - V_{residual}}{V_0} \right)^\beta} \f]
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
		/// @brief Calculates flux out of a storage as a linear function of its volume.
		///
		/// This connection serves the same purpose as the old kinematic_wave connection, but the parameters
		/// are easier to explain
		///
		/// \f[ q = \frac{V - V_{residual}}{t_r} \f]
		/// where:
		/// - \f$V\f$ The actual volume of water stored in source
		/// - \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
		///   \f$t_r\f$ is the residence time in the source. 
		class LinearStorageConnection : public flux_connection {
		protected:
			std::weak_ptr<WaterStorage> source;
			real calc_q(cmf::math::Time t);
			void NewNodes() {
				source = WaterStorage::cast(left_node());
			}
		public:
			/// @brief Linear flow parameter traveltime in days
			real residencetime;
			/// @brief residual volume of source
			real residual;

			/// @brief Creates a linear storage connection or Nash-box.
			/// \f[ q = \frac{V - V_{residual}}{t_r}} \f]
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param residencetime \f$t_r [days]\f$ The residence time of the water in this storage
			/// @param residual \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
			LinearStorageConnection(WaterStorage::ptr source, flux_node::ptr target, real residencetime, real residual = 0.0);
		};


		/// @ingroup connections
		/// @brief Calculates flux out of a storage as a linear function of its volume to a power.
		///
		/// This connection serves the same purpose as the old kinematic_wave connection, but the parameters
		/// are easier to explain.
		///
		/// \f[ q = Q_0 {\left(\frac{V - V_{residual}}{V_0} \right)^\beta} \f]
		/// where:
		/// - \f$V\f$ The actual volume of water stored in source
		/// - \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
		/// - \f$V_0\f$ A reference volume to scale the outflux. One can see \f$V_0\f$ as the inflection point of the outflow curve
		/// - \f$\beta\f$ A parameter to shape the response curve. 
		///   \f$Q_0\f$ is the outflow from the source in \f$\frac{m^3}{day}\f$, when \f$V = V_0\f$. 
		///
		/// @warning \f$\beta < 0.5\f$ may lead to numerical troubles and have a dubious hydrological meaning. Please avoid.
		class PowerLawConnection : public flux_connection {
		protected:
			std::weak_ptr<WaterStorage> source;
			real calc_q(cmf::math::Time t);
			void NewNodes() {
				source = WaterStorage::cast(left_node());
			}
		public:
			/// @brief outflow from the source in \f$\frac{m^3}{day}\f$, when \f$V = V_0\f$. 
			real Q0;
			/// @brief Shape parameter of the curve, \f$\beta\f$
			real beta;
			/// @brief residual volume of source
			real residual;
			/// @brief Reference volume \f$V_0\f$
			real V0;

			/// @brief Creates a power law connection.
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param Q0 Reference flow \f$Q_0 = q(V_0)\f$ Outflow when the source storage equals the reference volume
			/// @param V0 Reference volume \f$V_0\f$ The reference volume to scale the exponent
			/// @param beta \f$\beta [-]\f$ An empirical exponent to shape the flux function (default = 1 (linear function))
			/// @param residual \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
			PowerLawConnection(WaterStorage::ptr source, flux_node::ptr target, real Q0, real V0,
						 real beta = 1.0, real residual = 0.0);
		};
		/// @ingroup connections
		/// @brief A conceptual TOPmodel inspired connection
		///
		/// \f[ q = Q_0 \cdot e^{(V-V_0)/m} \f]
		///
		class ExponentialDeclineConnection : public flux_connection
		{
		protected:
			std::weak_ptr<WaterStorage> source;
			real calc_q(cmf::math::Time t);
			void NewNodes() {
				source = WaterStorage::cast(left_node());
			}
		public:
			/// @brief outflow from the source in \f$\frac{m^3}{day}\f$, when \f$V = V_0\f$. 
			real Q0;
			/// @brief Shape parameter of the curve, \f$\beta\f$
			real m;
			/// @brief Reference volume \f$V_0\f$
			real V0;
			/// @brief creates the exponential decline connection
			ExponentialDeclineConnection(WaterStorage::ptr source, flux_node::ptr target, real Q0, real V0, real m);

		};
		/// @ingroup connections
		/// @brief A conceptual flux between two storages that can be positive as well as negative
		/// @note The state of the right node is not monitored, hence negative volumes of the right node can occur!
		/// @deprecated Behaviour unclear, will be removed
		/// \f[ q = q_{spill}^*-q_{suc}^* \\
		/// q_{spill}^* = q_{spill} \left(\frac{V-V_{spill,min}}{V_{spill,min}}\right)^{\beta_{spill}} \\
		/// q_{suc}^* = q_{suc} \left(\frac{V_{suc,max}-V}{V_{suc,max}}\right)^{\beta_{suc}} \f]
		///
		/// where:
		///  - \f$q\f$ is the flow to the target
		///  - \f$q_{spill}^*\f$ is the actual spill flow to the target 
		///  - \f$q_{spill}\f$ is the spill flow at \f$V = 2V_{spill}\f$
		///  - \f$q_{suc}^*\f$ is the actual suction flow from the target
		///  - \f$q_{suc}\f$ is the sucked flow from the target when \f$V=0.0\f$
		///  - \f$\beta\f$ is a shape forming exponent for spill and suction flow.
		class bidirectional_kinematic_exchange : public flux_connection {
		protected:
			std::weak_ptr<WaterStorage> source;
			real calc_q(cmf::math::Time t);
			void NewNodes() {
				source = WaterStorage::cast(left_node());
			}
		public:
			/// @brief Suction starts below this threshold volume of source 
			real Vmaxsuc;
			/// @brief Spilling starts above this threshold volume of source 
			real Vminspill;
			/// @brief Spill flow at 2*Vminspill in m3/day
			real qspill;
			/// @brief Suction flow at V=0 m3
			real qsuc;
			/// @brief Exponent for suction
			real beta_suc;
			/// @brief Exponent for spilling
			real beta_spill;
			/// @brief Creates a kinematic wave connection.
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param Vmaxsuc Suction starts below this threshold volume of source 
			/// @param Vminspill Spilling starts above this threshold volume of source 
			/// @param qspill Spill flow at 2*Vminspill in m3/day
			/// @param qsuc Suction flow at V=0 m3
			/// @param beta_suc,beta_spill Exponent for spill / suction flow
			bidirectional_kinematic_exchange(WaterStorage::ptr source,flux_node::ptr target,
											real Vminspill,real Vmaxsuc,
											real qspill,real qsuc,
											real beta_spill,real beta_suc);
		};
		/// @ingroup connections
		/// @brief Calculates flux out of a storage as a linear function of its volume to a power, constraint by the volume stored in the target storage.
		/// @deprecated Will be replaced by ConstraintLinearStorageConnection, without beta and gamma.
		/// \f[ q = \frac 1 {t_r} {\left(\frac{V_{l} - V_{residual}}{V_0} \right)^\beta} \left(\frac{V_{r,max}-V_{r}}{V_{r,max}}\right)^\gamma\f]
		/// where:
		/// - \f$V_l\f$ The actual volume stored by the left water storage
		/// - \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
		/// - \f$V_0\f$ The reference volume to scale the exponent (default = 1m3/day)
		/// - \f$\beta\f$ A parameter to shape the response curve. In case of \f$\beta \neq 1\f$, 
		///   \f$t_r\f$ is not a residence time, but just a parameter.
		/// - \f$t_r [days]\f$ The residence time of the water in this storage in days
		/// - \f$V_{r,max}\f$ The capacity of the right water storage in m3
		/// - \f$V_{r}\f$ The actual volume of the right water storage
		/// - \f$\gamma\f$ A shape parameter for the target capacity constriction
		class constraint_kinematic_wave : public flux_connection {
		protected:
			WaterStorage::ptr source,target;
			real calc_q(cmf::math::Time t);
			void NewNodes() {
				source = WaterStorage::cast(left_node());
				target = WaterStorage::cast(right_node());
			}
		public:
			/// @brief Linear flow parameter traveltime in days
			real residencetime;
			/// @brief Exponent of volume, \f$\beta\f$
			real beta;
			/// @brief residual fraction \f$f_r\f$ of reference volume V0
			real residual;
			/// @brief Reference volume \f$V_0\f$
			real V0;
			/// @brief Target capacity \f$V_{r,max}\f$
			real Vrmax;
			/// @brief Target capacity constriction curve shape
			real gamma;
			/// @brief Creates a kinematic wave connection.
			/// \f[ q = \frac 1 {t_r} {\left(\frac{V - V_{residual}}{V_0} \right)^\beta} \f]
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param residencetime \f$t_r [days]\f$ The residence time of the water in this storage
			/// @param exponent \f$\beta [-]\f$ An empirical exponent to shape the flux function (default = 1 (linear function))
			/// @param residual \f$V_{residual} [m^3]\f$ The volume of water not flowing out (default = 0)
			/// @param V0 \f$V_0\f$ The reference volume to scale the exponent
			/// @param Vrmax \f$V_{r,max}\f$ Capacity of the target water storage in m3
			/// @param gamma \f$\gamma\f$ Target capacity constriction curve shape
			constraint_kinematic_wave(WaterStorage::ptr source,WaterStorage::ptr target,real residencetime=1.0,
				real exponent=1.0,real residual=0.0,real V0 = 1.0, real Vrmax = 1.0, real gamma=1.0);
		};

		/// @ingroup connections
		/// @brief Produces a constant but changeable flux from a source to a target, if enough water is present in the source
		///
		/// \f[ q=\begin{cases}0 & V_{source}\le V_{min}\\ q_0 \frac{V_{source} - V_{min}}{t_{decr} q_{0} - V_{min}} & V_{source} \le t_{decr} q_{0}\\ q_{0} &  \end{cases}\f]
		///
		/// This is similar to a neumann boundary, however this is not a boundary condition, but water is taken from the source (left) water storage and limited by that water storage.
		class TechnicalFlux : public cmf::water::flux_connection
		{
		protected:
			std::weak_ptr<cmf::water::WaterStorage> source;
			virtual real calc_q(cmf::math::Time t)	{
				return piecewise_linear(source.lock()->get_state(),MinState,MinState+FluxDecreaseTime.AsDays()*MaxFlux,0,MaxFlux);
			}
			void NewNodes()	{
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
		/// but there are no restrictions concerning the type of nodes. However, the left side needs to be a water storage
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
		/// where:
		/// - \f$q\f$ the resulting flux in m3/day
		/// - \f$h_1\f$ the reference state
		/// - \f$h_{target}\f$ the state of the target (right) node
		/// - \f$t_c\f$ the time to reach the target state
		class statecontrol_connection 
			: public flux_connection 
		{
		protected:
			std::weak_ptr<cmf::water::WaterStorage> source;
			virtual real calc_q(cmf::math::Time t);
			void NewNodes();
		public:
			cmf::math::Time reaction_time;
			real target_state;
			/// @brief Creates a flux connection to control the state of a storage
			///
			/// @param controlled_storage Water storage, to be controlled
			/// @param other_end source of missing water or target of excessive water
			/// @param target_state State the controlled storage should hold (\f$h_{target}\f$)
			/// @param reaction_time Time to reach state (\f$t_c\f$)
			statecontrol_connection(cmf::water::WaterStorage::ptr controlled_storage, cmf::water::flux_node::ptr other_end, real target_state, cmf::math::Time reaction_time );
		};


	}
	
}
#endif // simple_connections_h__
