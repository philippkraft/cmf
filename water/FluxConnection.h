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
#include "FluxNode.h"
namespace cmf {
	class project;
	namespace water {
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
