#ifndef system_bridge_h__
#define system_bridge_h__

#include "flux_node.h"
#include "flux_connection.h"
namespace cmf {
	namespace water {
		/// A SystemBridge is an advanced feature for tuning of the calculation time.
		///
		/// A SystemBridge can be used to replace an existing connection between nodes. 
		/// It is created using the system_bridge function. After installation, the two 
		/// nodes can more safely be added to different integrator systems. 
		/// One node (called upper) is connected with the system bridge with the connection formerly connecting the nodes,
		/// the second node (called lower) is connected to the system bridge with as a Neumann boundary condition. The flux equals the average flux
		/// of the connection upper <-> SystemBridge. Therefore, the downward flux needs to be integrated over time by the solver
		/// the upper node belongs to. Use as an upper system (system upper node is belonging to) the faster reacting system.
		/// For the connection between upper and SystemBridge, the SystemBridge reacts as an Dirichlet boundary condition, providing the potential 
		/// of the lower node.
		///
		/// The following example code creates a system bridge between the nodes upper and lower. 
		/// To integrate the flux over each timestep automatically, the systembridge is added
		/// to the solver of upper, as an integratable
		/// @code
		/// sys_bridge = cmf.system_bridge(p,upper,lower)
		/// upper_solver.add_integratable(sys_bridge.down_flux_integrator())
		/// @endcode
		class SystemBridge : public cmf::water::flux_node
		{
		private:
			flux_integrator::ptr m_ci;
			flux_node::ptr lower_node,upper_node;
			SystemBridge(cmf::project& p, flux_connection& con);
		public:
			typedef std::shared_ptr<SystemBridge> ptr;
#ifndef SWIG
			friend SystemBridge::ptr system_bridge(cmf::project&, flux_node::ptr,flux_node::ptr);
#endif


			/// Returns the upper node
			flux_node::ptr get_upper_node() const {return upper_node;}
			
			/// Returns the lower node
			flux_node::ptr get_lower_node() const {return lower_node;}
			double get_potential() const {
				return lower_node->get_potential();
			}

			/// Returns the currently integrated flux to the lower node
			double get_down_flux() const {
				return m_ci->avg();
			}
			flux_integrator::ptr down_flux_integrator() const {
				return m_ci;
			}

		};

		/// Connects a system bridge with its lower node. Is created automatically when creating a SystemBridge
		class SystemBridgeConnection 
			: public cmf::water::flux_connection 
		{
		protected:
#ifndef SWIG
			friend SystemBridge::ptr system_bridge(cmf::project&, flux_node::ptr,flux_node::ptr);
#endif
			std::weak_ptr<SystemBridge> sb;
			virtual real calc_q(cmf::math::Time t) {

				return sb.lock()->get_down_flux();
			}
			virtual void NewNodes() {
				sb=std::dynamic_pointer_cast<SystemBridge>(left_node());
			}
		private:
			SystemBridgeConnection(SystemBridge::ptr left,flux_node::ptr right)
				: flux_connection(left,right,"system bridge") {}
		};

		/// Creates a SystemBridge object. This is an advanced feature for tuning of the calculation time.
		///
		/// A SystemBridge can be used to replace an existing connection between nodes. 
		/// After installation, the two nodes can more safely be added to different integrator systems. 
		/// One node (called upper) is connected with the system bridge with the connection formerly connecting the nodes,
		/// the second node (called lower) is connected to the system bridge with as a Neumann boundary condition. The flux equals the average flux
		/// of the connection upper <-> SystemBridge. Therefore, the system bridge must become an integratable of the integrator system
		/// the upper node belongs to. Use as an upper system (system upper node is belonging to) the faster reacting system.
		/// For the connection between upper and SystemBridge, the SystemBridge reacts as an Dirichlet boundary condition, providing the potential 
		/// of the lower node.
		SystemBridge::ptr system_bridge(cmf::project& p, flux_node::ptr upper,flux_node::ptr lower);



	}
	
}
#endif // system_bridge_h__
