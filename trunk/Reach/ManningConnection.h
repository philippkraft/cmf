#ifndef ManningConnection_h__
#define ManningConnection_h__

#include "../water/flux_connection.h"
#include "OpenWaterStorage.h"
#include "ReachType.h"
#include "../math/real.h"
#include "../Upslope/cell.h"
namespace cmf {
	namespace river {
		/// Calculates the flux between two open water bodies, using Manning's equation
		class Manning : public cmf::water::flux_connection
		{
		protected:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,bool is_diffusive_wave);
			std::tr1::weak_ptr<cmf::river::OpenWaterStorage> w1,w2;
			virtual real calc_q(cmf::math::Time t);
			void NewNodes()
			{
				w1=cmf::river::OpenWaterStorage::cast(left_node());
				w2=cmf::river::OpenWaterStorage::cast(right_node());
			}
			Manning(cmf::river::open_water_storage_ptr left,cmf::water::flux_node::ptr right,
							cmf::river::Channel reachtype,bool diffusive_wave)
				: flux_connection(left,right,diffusive_wave ? "Manning (diff. wave)" : "Manning (kin. wave)"),	flux_geometry(reachtype), is_diffusive_wave(diffusive_wave) {
					NewNodes();
			}

		public:
			bool is_diffusive_wave;
			cmf::river::Channel flux_geometry;
			

			
		};

		class Manning_Diffusive: public Manning
		{
		private:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,int dummy)
			{		cmf::river::Manning::connect_cells(c1,c2,false);		}

		public:
			static const cmf::upslope::CellConnector cell_connector;
			Manning_Diffusive(cmf::river::open_water_storage_ptr left,cmf::water::flux_node::ptr right,cmf::river::Channel reachtype)
				: Manning(left,right,reachtype,false)
			{			}
		};
		class Manning_Kinematic: public Manning
		{
		private:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,int dummy)
			{		cmf::river::Manning::connect_cells(c1,c2,true);		}

		public:
			static const cmf::upslope::CellConnector cell_connector;
			Manning_Kinematic(cmf::river::open_water_storage_ptr left,cmf::water::flux_node::ptr right,cmf::river::Channel reachtype)
				: Manning(left,right,reachtype,true)
			{			}
		};
	}
}
#endif // ManningConnection_h__
