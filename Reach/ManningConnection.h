#ifndef ManningConnection_h__
#define ManningConnection_h__

#include "../water/FluxConnection.h"
#include "../water/WaterStorage.h"
#include "VolumeHeightRelation.h"
#include "ReachType.h"
#include "../math/real.h"
#include "../Upslope/cell.h"
namespace cmf {
	namespace river {
		class OpenWaterStorage : public cmf::water::WaterStorage
		{
		private:
			std::auto_ptr<VolumeHeightRelation> v_to_height;
		protected:
			OpenWaterStorage(const cmf::project& _project,cmf::river::VolumeHeightRelation* base_geo) :WaterStorage(_project), v_to_height(base_geo) {}
		public:
			void SetBaseTopography(const cmf::river::VolumeHeightRelation& new_base_topo) 
			{v_to_height.reset(new_base_topo.copy());}
			const cmf::river::VolumeHeightRelation& BaseTopography() const
			{return *v_to_height;}
			real h() const {return v_to_height->h(maximum(0,get_state()));}
			real wet_area() const {return v_to_height->A(maximum(0,get_state()));}
			real get_potential() const {return Location.z+h();} 
			OpenWaterStorage(const cmf::project& _project,real Area) : cmf::water::WaterStorage(_project,0), v_to_height(new cuboid(Area)) {}
			OpenWaterStorage(const cmf::project& _project,const cmf::river::VolumeHeightRelation& base_geo) : cmf::water::WaterStorage(_project,0), v_to_height(base_geo.copy()) {}
			static OpenWaterStorage* FromNode(cmf::water::FluxNode& node,real Area);
			static OpenWaterStorage* cast(cmf::water::FluxNode& node)
			{
				OpenWaterStorage* res=dynamic_cast<OpenWaterStorage*>(&node);
				return res;
			}
		};
		cmf::river::OpenWaterStorage* AsOpenWater(cmf::water::FluxNode* node);

		class Manning : public cmf::water::FluxConnection
		{
		private:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,int dummy);
		protected:
			cmf::river::OpenWaterStorage *w1,*w2;
			virtual real calc_q(cmf::math::Time t);
			void NewNodes()
			{
				w1=AsOpenWater(m_left);
				w2=AsOpenWater(m_right);
			}

			std::auto_ptr<ReachType> flux_geometry;
			real distance;
		public:
			
			ReachType& ChannelGeometry() {return *flux_geometry;}
			
			const ReachType& ChannelGeometry() const {return *flux_geometry;}
			static const cmf::upslope::CellConnector cell_connector;
			Manning(cmf::river::OpenWaterStorage& left,cmf::water::FluxNode & right,const ReachType& reachtype,real Distance)
				: FluxConnection(left,right,"Manning"),distance(Distance),	flux_geometry(reachtype.copy()) {
					NewNodes();
			}
			Manning(cmf::river::OpenWaterStorage& left,cmf::water::FluxNode & right,char reachtype,real width,real Distance)
				: FluxConnection(left,right,"Manning"),distance(Distance),	flux_geometry(create_reachtype(reachtype,width)) {
					NewNodes();
			}
			
		};

		class Manning_Kinematic: public cmf::water::FluxConnection
		{
		private:
			static void connect_cells(cmf::upslope::Cell& c1,cmf::upslope::Cell& c2,int dummy);
		protected:
			cmf::river::OpenWaterStorage *w1,*w2;
			virtual real calc_q(cmf::math::Time t);
			void NewNodes()
			{
				w1=AsOpenWater(m_left);
				w2=AsOpenWater(m_right);
			}

		public:
			real distance;
			real flow_width;
			real nManning;

			static const cmf::upslope::CellConnector cell_connector;
			Manning_Kinematic(cmf::river::OpenWaterStorage& left,cmf::water::FluxNode & right, real FlowWidth,real Distance,real n_Manning=0.035)
				: FluxConnection(left,right,"Kinematic wave surface flow"),distance(Distance),flow_width(FlowWidth),nManning(n_Manning) {
					NewNodes();
			}

		};

		/// Produces a constant but changeable flux from a source to a target, if enough water is present in the source
		///
		/// \f$ q=\left\{0 \mbox{ if }V_{source}\le V_{min}\\ \frac{V_{source} - V_{min}}{t_{decr} q_{0} - V_{min}}\mbox{ if } V_{source} t_{decr} q_{0}\\ q_{0} \mbox{ else}\le \right. \f$
		class TechnicalFlux : public cmf::water::FluxConnection
		{
		protected:
			cmf::water::WaterStorage *source;
			virtual real calc_q(cmf::math::Time t)
			{
				return piecewise_linear(source->get_state(),MinState,MinState+FluxDecreaseTime.AsDays()*MaxFlux,0,MaxFlux);
			}
			void NewNodes()
			{
				source=cmf::water::AsWaterStorage(m_left);
			}

		public:
			/// The requested flux \f$q_{0}\left[frac{m^3}{day}\right]\f$
			real MaxFlux;
			/// The minimal volume of the state  \f$V_{min}\left[m^3\right]\f$
			real MinState;
			/// The flux is linearly decreased, if it takes less than FluxDecreaseTime \f$t_{decr}\f$ to reach MinState with MaxFlux
			cmf::math::Time FluxDecreaseTime;

			/// Produces a constant but changeable flux from a source to a target, if enough water is present in the source
			/// @param source The source of the water
			/// @param target The target of the water
			/// @param maximum_flux The requested flux \f$q_{0}\f$
			/// @param minimal_state Minimal volume of stored water in source
			/// @param flux_decrease_time (cmf::math::Time)
			TechnicalFlux(cmf::water::WaterStorage & source,cmf::water::FluxNode & target,real maximum_flux,real minimal_state=0,cmf::math::Time flux_decrease_time=cmf::math::h)
				: FluxConnection(source,target,"Technical flux"),MaxFlux(maximum_flux),MinState(minimal_state),FluxDecreaseTime(flux_decrease_time) {}
		};
	
	}
}
#endif // ManningConnection_h__
