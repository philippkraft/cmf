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
#include <stdexcept>
#include "../math/numVector.h"

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
		class FluxNode : public cmf::geometry::Locatable {
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

			cmf::geometry::point get_position() const
			{
				return Location;
			}
			void set_position(cmf::geometry::point p)
			{
				Location=p;
			}

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
			cmf::geometry::point flux3d_to(const cmf::water::FluxNode& target,cmf::math::Time t );
			cmf::geometry::point get_3d_flux(cmf::math::Time t);
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
		/// Transfers all connections of old node to new node. Not intended for external use. If needed, use carefully,
		/// and be prepared for memory corruption
		void replace_node(cmf::water::FluxNode& oldnode,cmf::water::FluxNode& newnode);

		
		/// A collection of nodes for fast access of the waterbalance
		///
		/// In setups with many storages and rather fast computations, the speed of data access for output generation can take a high portion of the total run time.
		/// To accelerate data access, one can use the node_list object
		class node_list
		{
		private:

			typedef std::vector<cmf::water::FluxNode*> node_vector;
			node_vector m_nodes;
		public:
			/// The number of nodes
			int size() const {return int(m_nodes.size());}
#ifndef SWIG
			FluxNode* operator[](int index) const
			{
				return m_nodes.at(index<0 ? size()+index : index);
			}

#endif
			node_list& operator+=(const node_list& right)
			{
				for (int i = 0; i < right.size() ; ++i)
				{
					append(right[i]);
				}
				return *this;
			}
			FluxNode* get(int index) const
			{
				return m_nodes.at(index<0 ? size()+index : index);
			}
			node_list get(int begin,int end,int step=1) const
			{
				node_list res;
				for (int i = begin; i <end  ; i+=step)
				{
					res.append(get(i));
				}
				return res;
			}
			/// Adds a flux node to the list
			void append(FluxNode* node)
			{
				if (node==0) throw std::invalid_argument("node_list may only contain valid objects");
				m_nodes.push_back(node);
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
			/// A fast method to perform flux queries as a batch. The node lists left and right should have the same length.
			///
			/// @returns The vector containing the flux from left to right at the same position
			/// @param right A node_list containing the source nodes
			/// @param t The time for the fluxes
			///
			/// Replaces slow Python code like:
			/// @code
			/// fluxes=[left_node[i].flux_to(right_node[i],t) for i in range(len(left_nodes))]
			/// @endcode
			cmf::math::numVector get_fluxes_to( const cmf::water::node_list& targets ,cmf::math::Time t) const;
			cmf::geometry::point_vector get_fluxes3d_to( const cmf::water::node_list& targets ,cmf::math::Time t) const;
			cmf::geometry::point_vector get_fluxes3d(cmf::math::Time t) const;
		};


	}

}
#endif // FluxNode_h__
