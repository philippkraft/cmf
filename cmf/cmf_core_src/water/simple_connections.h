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
		class WaterbalanceFlux : public flux_connection
		{
		protected:
			real calc_q(cmf::math::Time t) override;
			void NewNodes() {}
		public:
			WaterbalanceFlux(flux_node::ptr source, flux_node::ptr target);
		};

        /// @ingroup connections
        /// @brief Routes a fraction of the flux calculated from a master flux_connection between source to target1
        /// directly further to target2 without any timelag. The connection connects target 1 and target 2.
        ///
        /// \f[ q_{t1,t2} = f \cdot q_{s, t1}(t) \f]
        ///
        /// Where \f$ q_{t1,t2}\f$ is the flux from t1 to t2, f is the fraction and \f$ q_{s,t1}(t)\f$ is the original
        /// flux
        class PartitionFluxRoute : public flux_connection
        {
        private:
            std::weak_ptr<flux_node> m_source;
        protected:
            real calc_q(cmf::math::Time t) override;
            void NewNodes() {}
        public:
            /// @param source Water storage from which the water flows out. The flux is defined by some other flux connection between
            /// @param target1 Target node (boundary condition or storage). The target of the master flux connection
            /// @param target2 Target node (boundary condition or storage). Does not influence the strength of the flow
            /// @param fraction Fraction of the source->target1 flow to be routed further to target2
            /// @param no_back_flow If true (default), no flow between target 1 and target 2 occurs, if water is flowing from
            ///                     target 1 to source. If set to false, in the case of water flowing from target 1 to source,
            ///                     the fraction is also flowing from target 2 to target 1. No test if target 2 has water is made.
            PartitionFluxRoute(flux_node::ptr source, flux_node::ptr target1, flux_node::ptr target2, real fraction, bool no_back_flow=true);
            flux_node::ptr source() const {
                return m_source.expired() ? flux_node::ptr() : flux_node::ptr(m_source);
            }
            real fraction;
            bool no_back_flow;
        };

        /// @ingroup connections
		/// @brief Flux from one node to another, controlled by the user or an external program, 
		/// by changing the flux constant
		///
		/// @note It is easy to create negative volumes in water storages with this connection, 
		/// which can be hazard to the solver, since most connections rely on a positive volume in a storage.
		/// Handle with care!
		class ExternallyControlledFlux : public flux_connection {
		protected:
			real calc_q(cmf::math::Time t)	{
				return flux;
			}
			void NewNodes() {}
		public:
			real flux;
			ExternallyControlledFlux(flux_node::ptr source,flux_node::ptr target,real flux_value=0)
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
		/// @brief Calculates flux out of a storage as a linear function of its volume, constraint by the volume stored in the target storage.
		/// \f[ q = \frac {1}{t_r} \left({V_{l} - V_{l,min}}\right) \cdot \left(\frac{V_{r,max}-V_{r}}{V_{r,max}}\right)\f]
		/// where:
		/// - \f$V_l\f$ The actual volume stored by the left water storage
		/// - \f$V_{l,min} [m^3]\f$ The volume of water not flowing out (default = 0)
		/// - \f$\beta\f$ A parameter to shape the response curve. In case of \f$\beta \neq 1\f$, 
		///   \f$t_r\f$ is not a residence time, but just a parameter.
		/// - \f$t_r [days]\f$ The residence time of the water in this storage in days
		/// - \f$V_{r,max}\f$ The capacity of the right water storage in m3
		/// - \f$V_{r}\f$ The actual volume of the right water storage
		/// - \f$\gamma\f$ A shape parameter for the target capacity constriction
		class ConstraintLinearStorageFlux : public flux_connection {
		protected:
			WaterStorage::ptr source, target;
			real calc_q(cmf::math::Time t);
			void NewNodes();
		public:
			/// @brief Linear flow parameter traveltime in days
			real residencetime;
			/// @brief residual volume \f$V_{l,min}\f$ in left storage
			real Vlmin;
			/// @brief Target capacity \f$V_{r,max}\f$
			real Vrmax;
			/// @brief Creates a linear storage connection, constrained by the fill level of the source
			/// \f[ q = \frac 1 {t_r} {\left(\frac{V - V_{residual}}{V_0} \right)^\beta} \f]
			/// @param source Water storage from which the water flows out. Flux is a function of source.volume
			/// @param target Target node (boundary condition or storage). Does not influence the strength of the flow
			/// @param residencetime \f$t_r [days]\f$ The residence time of the water in this storage
			/// @param Vlmin \f$V_{l,min} [m^3]\f$ The volume of water not flowing out (default = 0)
			/// @param Vrmax \f$V_{r,max}\f$ Capacity of the target water storage in m3
			ConstraintLinearStorageFlux(WaterStorage::ptr source, WaterStorage::ptr target,
				real residencetime = 1.0, real Vlmin = 0.0, real Vrmax = 1.0);
		};

        /// @ingroup connections
		/// @brief Produces a constant but changeable flux from a source to a target, if enough water is present in the source
		///
		/// \f[ q=\begin{cases}0 & V_{source}\le V_{min}\\ q_0 \frac{V_{source} - V_{min}}{t_{decr} q_{0} - V_{min}} & V_{source} \le t_{decr} q_{0}\\ q_{0} &  \end{cases}\f]
		///
		/// This is similar to a neumann boundary, however this is not a boundary condition, but water is taken from the source (left) water storage and limited by that water storage.
		class ConstantFlux : public cmf::water::flux_connection
		{
		protected:
			std::weak_ptr<cmf::water::WaterStorage> source;
			real calc_q(cmf::math::Time t) override	{
				return piecewise_linear(source.lock()->get_state(),MinState,MinState+FluxDecreaseTime.AsDays()*MaxFlux,0,MaxFlux);
			}
			void NewNodes() override	{
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
			/// @param maximum_flux The requested flux \f$q_{0} [\frac{m^3}{day}]\f$
			/// @param minimal_state Minimal volume of stored water in source in \f$[m^3]\f$
			/// @param flux_decrease_time (cmf::math::Time)
			ConstantFlux(cmf::water::WaterStorage::ptr source,cmf::water::flux_node::ptr target,real maximum_flux,real minimal_state=0,cmf::math::Time flux_decrease_time=cmf::math::h)
				: flux_connection(source,target,"Technical flux"),MaxFlux(maximum_flux),MinState(minimal_state),FluxDecreaseTime(flux_decrease_time) {
					NewNodes();
			}
		};

		/// @brief A generic node-to-node gradient based connection. 
		
		/// This connection is similar to the Darcy-connection,
		/// but there are no restrictions concerning the type of nodes. However, the left side needs to be a water storage
		/// \f[
		/// q = Q_1 \nabla \Psi
		/// \f]
		/// where:
		/// - \f$q\f$: the resulting flux in \f$m^3/day\f$
		/// - \f$Q_1\f$: Flux over the connection for a unity gradient (@f$\nabla \Psi = 1@f$) in @f$\frac{m^3}{day}@f$
		/// - \f$\nabla \Psi =  \frac{\Psi_{l}-\Psi_{r}}{d}\f$: The hydraulic gradient of the (l)eft,
		///     resp. (r)ight node of the connection
		/// - \f$d\f$: The topographic length of the connection in m
		class LinearGradientFlux
			: public flux_connection
		{
		protected:
			real calc_q(cmf::math::Time t) override;
			void NewNodes() override {}
		public:
			/// Flux over the connection for a unity gradient (@f$\nabla \Psi = 1 @f$) in @f$\frac{m^3}{day}@f$
			real Q1;
			/// distance of the gradient (use negative value for calculation from the node positions)
			real d;
			/// @brief Creates a generic gradient based flux, if enough water is present in the source
			///
			/// @param left The left node of the connection
			/// @param right The right node of the connection
			/// @param Q1 Flux over the connection for a unity gradient (@f$\nabla \Psi = 1@f$) in @f$\frac{m^3}{day}@f$
			/// @param d the topographic lenght of the connection in m
			LinearGradientFlux(cmf::water::flux_node::ptr left,cmf::water::flux_node::ptr right, real Q1,real d=-1.0);
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
		class ConstantStateFlux
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
			ConstantStateFlux(cmf::water::WaterStorage::ptr controlled_storage, cmf::water::flux_node::ptr other_end, real target_state, cmf::math::Time reaction_time );
		};


	}
	
}
#endif // simple_connections_h__
