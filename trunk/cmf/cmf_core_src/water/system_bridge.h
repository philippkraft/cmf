#ifndef system_bridge_h__
#define system_bridge_h__

#include "flux_node.h"
#include "flux_connection.h"
namespace cmf {
	namespace water {
		/// A SystemBridge is an advanced feature for tuning of the calculation time.
		///
		/// A SystemBridge can be used to replace an existing connection between nodes. It is created using the system_bridge function
		/// After installation, the two nodes can more safely be added to different integrator systems. 
		/// One node (called upper) is connected with the system bridge with the connection formerly connecting the nodes,
		/// the second node (called lower) is connected to the system bridge with as a Neumann boundary condition. The flux equals the average flux
		/// of the connection upper <-> SystemBridge. Therefore, the system bridge must become an integratable of the integrator system
		/// the upper node belongs to. Use as an upper system (system upper node is belonging to) the faster reacting system.
		/// For the connection between upper and SystemBridge, the SystemBridge reacts as an Drichelet boundary condition, providing the potential 
		/// of the lower node.
		class SystemBridge : public cmf::water::flux_node , public cmf::math::integratable
		{
		private:
			std::auto_ptr<connection_integrator> m_ci;
			flux_node::ptr lower_node,upper_node;
			SystemBridge(cmf::project& p, flux_connection& con);
		public:
			typedef std::tr1::shared_ptr<SystemBridge> ptr;
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
			virtual void integrate(cmf::math::Time t) {
				m_ci->integrate(t);
			}

			virtual void reset(cmf::math::Time t) {
				m_ci->reset(t);
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
			std::tr1::weak_ptr<SystemBridge> sb;
			virtual real calc_q(cmf::math::Time t) {

				return sb.lock()->get_down_flux();
			}
			virtual void NewNodes() {
				sb=std::tr1::dynamic_pointer_cast<SystemBridge>(left_node());
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
		/// For the connection between upper and SystemBridge, the SystemBridge reacts as an Drichelet boundary condition, providing the potential 
		/// of the lower node.
		SystemBridge::ptr system_bridge(cmf::project& p, flux_node::ptr upper,flux_node::ptr lower);



	}
	
}
#endif // system_bridge_h__
