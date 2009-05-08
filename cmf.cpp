

#ifdef _CONSOLE
	
	// Include a little bit of everything for everybody
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "Atmosphere/Meteorology.h"
#include "Upslope/Cell.h"
#include "Upslope/vegetation/ET.h"
#include "math/Integrators/BDF2.h"
#include "math/Integrators/cvodeIntegrator.h"
#include "Upslope/Soil/RetentionCurve.h"
#include "Atmosphere/Precipitation.h"
#include "water/FluxConnection.h"
#include "Upslope/connections/subsurfacefluxes.h"
#include "Upslope/connections/surfacefluxes.h"
#include "Upslope/connections/infiltration.h"
#include "Upslope/vegetation/ShuttleworthWallace.h"
#include "Reach/reach.h"
#include "Reach/ManningConnection.h"
	/// Main function of the program. Only for debugging and testing, the real CMF<sub>lib</sub> will be compiled as a DLL and the 
	/// main function will be replaced by Python code
#include <ctime>
#include <cmath>
/// Prints calculation time measurements
void printTime(const std::string& text,clock_t lastClock)
{
	std::cout << text << ((double)(clock()-lastClock))/CLOCKS_PER_SEC << " s" << std::endl;
}
int main(int argc, char* argv[])
	{
		using namespace std;
		using namespace cmf;
		using namespace	cmf::atmosphere;
		using namespace	cmf::math;
		using namespace cmf::water;
		using namespace cmf::upslope;
		using namespace cmf::upslope::connections;
		using namespace cmf::upslope::ET;
		using namespace cmf::river;
		project p;
		p.debug=1;
		Cell& c=*p.NewCell(0,0,0,100);
		Cell& c2=*p.NewCell(10,0,0.5,100);
		c.get_topology().AddNeighbor(c2,10);
		c2.get_topology().AddNeighbor(c,10);

		Reach& r=c.AddReach(10);
		ReachIterator it(&r);
		cout << it.valid() << endl;
		c.AddStorage("",'W');
	  FluxNode outlet(p);
		outlet.Name="Outlet";
		r.set_outlet(outlet);
		OpenWaterStorage& sw= *AsOpenWater(&c.SurfaceWater());
		sw.State(10);
		double p_sw=sw.Potential();
		double p_r=r.get_water().Potential();
		Time t0;
		cout << "h=" << sw.h()*100 << "cm q=" << sw.FluxTo(r,t0) << endl;
		SinglePrecipitationTimeseries pdata(20.0);
		pdata.Data.step=day*2;
		pdata.Data.Add(0.0);
		pdata.Data.begin=day*180;
		p.SetPrecipitation(pdata);
		SingleMeteorology meteo;
		//VanGenuchtenMualem vgm(2.0,0.01,0.5,5);
		BrooksCoreyRetentionCurve bc;
		RCurve r_curve(bc,1);
		for (int i = 0; i < 1 ; ++i)
		{
			Cell* c=p.NewCell(i*10,0,i*0.5,100);
			if (i)
			{
				c->get_topology().AddNeighbor(p.Cell(-2),10);
				p.Cell(-2).get_topology().AddNeighbor(*c,10);
			}
// 			for (int j = 0; j < 0 ; ++j)
// 			{
// 				c->AddLayer((j+1)*0.1,r_curve);
// 				c->Layer(-1).SetPotential(-0.5);
// 			}
			c->AddVariableLayerPair(1.,r_curve);
			c->SetSaturatedDepth(0.5);
			//Richards::use_for_cell(*c);
			c->AddStorage("",'W');
			CanopyOverflow::use_for_cell(*c);
			SimpleTindexSnowMelt::use_for_cell(*c);
			ShuttleworthWallaceET::use_for_cell(*c);
			int scount=c->StorageCount();
			c->Vegetation.RootDepth=0.5;
			c->Vegetation.fraction_at_rootdepth=0.5;
			//new Rainfall(c->SurfaceWater(),*c);
			//new MatrixInfiltration(c->Layer(0),c->SurfaceWater());
		}
//		p.Connect(Richards::cell_connector);
		cmf::upslope::Cell& c0=p.Cell(0);
		//Cell& c1=p.Cell(-1);
		//OpenWaterStorage& c1_sw=*OpenWaterStorage::cast(c1.SurfaceWater());
		//SoilWaterStorage& c1_l0=c1.Layer(0);
		//FluxConnection* fc=c1_sw.Connection(c1.Layer(0));
		t0=day*180;
		meteo.T.begin=t0;
		meteo.Tmin.begin=t0;
		meteo.Tmax.begin=t0;
		meteo.Tmin.Add(-5);
		meteo.Tmax.Add(5);
		meteo.T.step=day*0.5;
		meteo.T.interpolationpower=1;
		meteo.T.Add(-5);
		meteo.T.Add(0);
		meteo.T.Add(5);
		p.SetMeteorology(meteo);
		//Cell& c2=*p.NewCell(10,0,0.5,100);
		//c1.AddNeighbor(c2,10);
		//c2.AddNeighbor(c1,10);
		//c1.AddVariableLayerPair(1,r_curve);
		//c2.AddVariableLayerPair(1,r_curve);
// 		for (int i = 0; i < 20 ; ++i)
// 		{
// 			c1.AddLayer((i+1)*0.05,r_curve);
// 			c1.Layer(-1).SetPotential(-1);
// 		}
//	Richards::use_for_cell(c1);
		CVodeIntegrator integ(p,1e-6);
		integ.ModelTime(day*180);
		//integ.preconditioner='R';
		//integ.MaxOrder=2;
		//integ.UseEulerAtTmin=1;
		cout.precision(3);
		connection_set cons=p.get_connections();
		real in_flux=0;
		//real V_start=c1.Layer(0).State()+c1.Layer(1).State()+c2.Layer(0).State()+c2.Layer(1).State();
		//c1_sw.State(10);
		Time t_last;
		for (int i = 0; integ.ModelTime() < day*184 ; integ.Integrate(integ.ModelTime()+math::h,math::h),++i)
		{
			Time t=integ.ModelTime();
			cout << t << " dt:" << integ.TimeStep() << " T=" << c0.Weather(t).T; 
			for(connection_set::iterator it = cons.begin(); it != cons.end(); ++it)
			{
			  FluxConnection& con=**it;  
				cout << " " << con.type << "=" << con.q(con.Target(1),t);
			}
// 			for (int i = 0; i < c0.StorageCount() ; ++i)
// 			{
// 				cout << " " << c0.GetStorage(i).Name << ": " << c0.GetStorage(i).State();
// 			}
			//cout << " h_sw=" << c1_sw.h() << " q_in=" << c1_sw.FluxTo(c1.Layer(0),t) ;
			/*cout << " W0=" << c1.Layer(0).Wetness() << " q0_out=" << c1.Layer(0).FluxTo(c1.Layer(1),t) ;
			cout << " q0_lat=" << c1.Layer(0).FluxTo(p.Cell(-2).Layer(0),t)  ;
					 /*<< " err_pos=" << integ.get_error_position() */
			cout << endl << "sd=" << c0.SaturatedDepth();
			cout << endl;
		}
		cin.get();
		

	}
#endif //Not def _DLL

