#ifndef FluxConnection_h__
#define FluxConnection_h__
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
		class FluxNode;
		typedef std::vector<cmf::water::FluxNode*> node_vector;
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
			virtual bool IsStorage() const {return false;}
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
			void RemoveConnection(const cmf::water::FluxNode & To);
			/// Returns the actual flux between this and target	(positive sign means "from target into this")
			real FluxTo( const cmf::water::FluxNode& target,cmf::math::Time t );
			/// Returns the sum of all fluxes (positive and negative) at time t. Single fluxes can be excluded from the calculation
			/// @param t Time of the query
			/// @param Without A FluxConnection that is excluded from the Waterbalance (e.g. to prevent closed circuits)
			real Waterbalance(cmf::math::Time t,const FluxConnection* Without=0);
			/// Returns the water quality of the FluxNode, if it is not overridden this is the mix of the incoming fluxes
			virtual cmf::water::WaterQuality conc(cmf::math::Time t) const;
			cmf::geometry::point Location;
			/// Returns the water potential of the node in m waterhead
			/// The base class water storage always returns the height of the location
			virtual real Potential() const
			{
				return Location.z;
			}
			virtual bool Empty()
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
		void replace_node(cmf::water::FluxNode& oldnode,cmf::water::FluxNode& newnode);
		/// Represents a connection between FluxNodes, where water fluxes occur. 
		class FluxConnection 
		{
		private:
			static int nextconnectionid;
			FluxConnection(const FluxConnection& copy):connection_id(nextconnectionid) {
				throw std::runtime_error("Never copy construct a FluxConnection");
			}
		protected:
			FluxNode * m_left;
			FluxNode * m_right;
			virtual void NewNodes()=0;
			bool RecalcAlways;
			/// this function needs to be overridden by realizations of connections
			virtual real calc_q(cmf::math::Time t) = 0;
			// Connection stores q to prevent unnecessary recalculations
			real m_q; // Positive q means flux left to right
			// returns the actual flux, recalculates it, if one of the two flux nodes requires it
			real q(cmf::math::Time t) {
				if (RecalcAlways || m_left->RecalcFluxes(t) || m_right->RecalcFluxes(t))
					m_q=calc_q(t);
				return m_q;
			}
		public:
			const int connection_id;
			bool operator==(const cmf::water::FluxConnection& other) {return connection_id==other.connection_id;}
			/// Returns the other end of a connection than the asking end
			FluxNode& Target(const FluxNode& inquirer) {
				if (&inquirer == m_left) return *m_right;
				else if (&inquirer==m_right) return *m_left;
				else throw std::out_of_range("Inquirer not part of connection");
			}
			FluxNode& Target(int index) const
			{
				if (index==0 || index==-2)
					return *m_left;
				else if (abs(index)==1)
					return *m_right;
				else
					throw std::out_of_range("Only indices 0 and 1 are valid for querying a connection target");

			}
			void ExchangeTarget(FluxNode& oldtarget,FluxNode& newTarget);
			/// Returns the other end of a connection than the asking end	(const)
			const FluxNode& Target(const FluxNode& inquirer) const {
				if (&inquirer == m_left) return *m_right;
				else if (&inquirer==m_right) return *m_left;
				else throw std::out_of_range("Inquirer not part of connection");
			}
			/// Returns the current flux through a connection. Negative signs mean out of the inquirer, positive are inflows to the inquirer
			real q(const FluxNode& inquirer,cmf::math::Time t) {
				return &inquirer==m_left ? -q(t) 
					: &inquirer==m_right ? q(t) : 0;  
			}
			/// Returns the concentration of the flux. If not overridden,
			/// it returns the concentration of the source of the flux (direction depending)
			WaterQuality conc(const FluxNode& inquirer,cmf::math::Time t)
			{
				real _q=q(inquirer,t);
				if (_q>0) return m_right->conc(t);
				else return m_left->conc(t);
			}
			const std::string type;
			std::string ToString() const
			{
				std::string from=" from ";
				std::string to=" to ";
				return type + from + m_left->Name + to + m_right->Name;
			}
			/// Creates a flux connection between the FLuxNode left and right
			/// @param left FluxNode on the one side of the connection
			/// @param right FluxNode on the other side of the connection
			FluxConnection(FluxNode& left,FluxNode& right,std::string _type);

			virtual ~FluxConnection();
		};


// 		class RouteWaterBalanceConnection : public FluxConnection
// 		{
// 		protected:
// 			real calc_q(cmf::math::Time t) 
// 			{
// 				return m_left->Waterbalance(t,this);
// 			}
// 		public:
// 			RouteWaterBalanceConnection(FluxNode& source,FluxNode& target) : FluxConnection(source,target,"RouteWaterBalance")
// 			{
// 				RecalcAlways=true;
// 			}
// 		};
	}
}

#endif // FluxConnection_h__
