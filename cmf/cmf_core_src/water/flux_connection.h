#ifndef flux_connection_h__
#define flux_connection_h__
#include "../math/Time.h"
#include "../math/real.h"
#include "../Geometry/geometry.h"
#include "Solute.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include "flux_node.h"
#include <stdexcept>
#include <tr1/memory>
namespace cmf {
	class project;
	namespace water {
		/// @defgroup connections Flux connections
		///
		/// The connections between the nodes (boundary conditions, storages) of the water network
		/// The connections in cmf hold the processes for the calculation of fluxes between water storages and model boundaries
		/// @todo Elaborate on this

		/// @ingroup connections
		/// Represents a connection between flux_nodes, where water fluxes occur. 
		class flux_connection 
		{
		protected:
			typedef std::tr1::weak_ptr<flux_node> weak_flux_node_ptr;
		private:
			friend class flux_node;
			static int nextconnectionid;
			flux_connection(const flux_connection& copy):connection_id(nextconnectionid) {
				throw std::runtime_error("Never copy construct a flux_connection");
			}
			std::tr1::weak_ptr<flux_connection> weak_this;
			weak_flux_node_ptr m_left;
			weak_flux_node_ptr m_right;
			real m_tracer_filter;
		protected:
			virtual void NewNodes()=0;
			bool RecalcAlways;
			/// this function needs to be overridden by realizations of connections
			virtual real calc_q(cmf::math::Time t) = 0;
			// Connection stores q to prevent unnecessary recalculations
			real m_q; // Positive q means flux left to right
			// returns the actual flux, recalculates it, if one of the two flux nodes requires it
			real q(cmf::math::Time t) {
				if (RecalcAlways || m_left.lock()->RecalcFluxes(t) || m_right.lock()->RecalcFluxes(t))
					m_q=calc_q(t);
				return m_q;
			}
		public:
			typedef std::tr1::shared_ptr<flux_connection> ptr;
			/// Returns the left node of this connection
			flux_node::ptr left_node() const {return m_left.expired() ? flux_node::ptr() : flux_node::ptr(m_left);}
			/// returns the right node of this connection
			flux_node::ptr right_node() const {return m_right.expired() ? flux_node::ptr() : flux_node::ptr(m_right);}
			
			/// Deregisters this connection from its nodes. Returns true if only one reference is left.
			bool kill_me();


			const int connection_id;
			
			bool operator==(const cmf::water::flux_connection& other) {return connection_id==other.connection_id;}
			
			/// Returns the other end of a connection than the asking end
			flux_node::ptr get_target(const flux_node& inquirer);
			/// With index 0, the left node is returned, with index 1 the right node of the connection
			flux_node::ptr get_target(int index) const;
			
			void exchange_target(flux_node::ptr oldtarget,flux_node::ptr newTarget);
			
			
			/// Returns the current flux through a connection. Negative signs mean out of the inquirer, positive are inflows to the inquirer
			real q(const flux_node& inquirer,cmf::math::Time t) {
				return inquirer==*left_node() ? -q(t) 
					: inquirer==*right_node() ? q(t) : 0;  
			}
			
			/// Returns the concentration of the flux. If not overridden,
			/// it returns the concentration of the source of the flux (direction depending)
			real conc(cmf::math::Time t, const cmf::water::solute& _Solute);
			/// A string describing the type of the connection
			const std::string type;
			/// A value ranging from 0 to 1 
			real get_tracer_filter() { return m_tracer_filter;}
			void set_tracer_filter(real value) { 
				if (value<0 || value>1) throw std::runtime_error("Tracer filter must be between 0 and 1");
				m_tracer_filter = value;
			}
			std::string to_string() const;
			std::string short_string() const;
			/// Creates a flux connection between the flux_node left and right
			/// @param left flux_node on the one side of the connection
			/// @param right flux_node on the other side of the connection
			/// @param _type Type of the flux connection
			flux_connection(flux_node::ptr left,flux_node::ptr right,std::string _type);

			virtual ~flux_connection();
		};

		void replace_node(cmf::water::flux_node::ptr oldnode,cmf::water::flux_node::ptr newnode);

		

		/// @ingroup connections
		/// Routes the sum of all other fluxes to a target
		class waterbalance_connection : public flux_connection
		{
		protected:
			real calc_q(cmf::math::Time t) 
			{
				real q = left_node()->water_balance(t,this);
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
	}
}

#endif // flux_connection_h__
