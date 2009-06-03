#ifndef FluxNode_h__
#define FluxNode_h__

#include "../math/Time.h"
#include "../math/real.h"
#include "../Geometry/geometry.h"
#include "Solute.h"
#include <map>
#include <vector>
#include <set>
#include <string>
namespace cmf {
	class project;
	namespace water {

		class FluxConnection;
		typedef std::vector<cmf::water::FluxConnection*> connection_vector;
		typedef std::set<cmf::water::FluxConnection*> connection_set;

		/// Base class for everything that can be connected by fluxes. 
		/// Flux nodes can be WaterStorages, flux end points, sinks, sources and 
		/// bridges to other model domains (e.g. Ponded water to river system).
		/// The base class can be used where a simple routing, potentially with mixing,
		/// is needed.
		class FluxNode {
		private:
			static int nextnodeid;
			void RegisterConnection(FluxConnection* newConnection);
			void DeregisterConnection(FluxConnection* oldConnection);
			friend class FluxConnection;
			typedef std::map<int,FluxConnection*> ConnectionMap;
			ConnectionMap m_Connections;
			const cmf::project& m_project;
		protected:
			cmf::math::Time m_LastQuery;
		public:
			const cmf::project& project() const {return m_project;}
			const int node_id;
			virtual bool is_storage() const {return false;}
			std::string Name;
			cmf::water::connection_vector Connections()
			{
				connection_vector res;
				for(ConnectionMap::iterator it = m_Connections.begin(); it != m_Connections.end(); ++it)
					res.push_back(it->second);
				return res;
			}
			/// The destructor deletes all connections
			virtual ~FluxNode();
			bool operator ==(const cmf::water::FluxNode& other) {return node_id==other.node_id;}

			/// Pure FluxNodes do not influence fluxes, therefore no recalculation of fluxes is required by fluxnode.
			/// WaterStorage overrides this, since state changes require an update of the fluxes
			virtual bool RecalcFluxes(cmf::math::Time t) {return false;}
			/// Returns the connection between this and target
			cmf::water::FluxConnection* Connection(const cmf::water::FluxNode& target);
			void remove_connection(const cmf::water::FluxNode & To);
			/// Returns the actual flux between this and target	(positive sign means "from target into this")
			real flux_to( const cmf::water::FluxNode& target,cmf::math::Time t );
			/// Returns the sum of all fluxes (positive and negative) at time t. Single fluxes can be excluded from the calculation
			/// @param t Time of the query
			/// @param Without A FluxConnection that is excluded from the water_balance (e.g. to prevent closed circuits)
			real water_balance(cmf::math::Time t,const FluxConnection* Without=0);
			/// Returns the water quality of the FluxNode, if it is not overridden this is the mix of the incoming fluxes
			virtual cmf::water::WaterQuality conc(cmf::math::Time t) const;
			cmf::geometry::point Location;
			/// Returns the water potential of the node in m waterhead
			/// The base class water storage always returns the height of the location
			virtual real get_potential() const
			{
				return Location.z;
			}
			virtual void set_potential(real new_potential)
			{
				throw std::runtime_error("Potential of "+Name+" is read only");
			}
			virtual bool is_empty()
			{
				return true;
			}
			FluxNode(const cmf::project& _project,cmf::geometry::point location=cmf::geometry::point());
			virtual FluxNode* copy() const
			{
				FluxNode* res=new FluxNode(m_project);
				res->Location=this->Location;
				return res;
			}
		};

		typedef std::vector<cmf::water::FluxNode*> node_vector;


		void replace_node(cmf::water::FluxNode& oldnode,cmf::water::FluxNode& newnode);

	}

}
#endif // FluxNode_h__
