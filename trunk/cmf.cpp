

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
#include "Upslope/algorithm.h"
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
		timeseries ts=1.0;
		timeseries ts2=ts;
		//NeumannBoundary nbc(p,ts2);
		p.debug=1;
		Cell& cell=*p.NewCell(0,0,0,100);
		BrooksCoreyRetentionCurve bc;
		cell.add_variable_layer_pair(1.,bc);
		//new VarLayerPercolationSimple(FlexibleSizeSaturatedZone::get_from_cell(cell)->UpperLayer(),*FlexibleSizeSaturatedZone::get_from_cell(cell));
		cell.set_saturated_depth(0.0);
		PenmanMonteithET::use_for_cell(cell);
		cmf::atmosphere::meteo_station_pointer meteo=p.meteo_stations.add_station("");
		meteo->Tmax.add(20);
		meteo->Tmin.add(10);
		cell.set_meteorology(MeteoStationReference(meteo,cell));
		cell.set_saturated_depth(0.5);
		//cell.get_layer(0).Wetness(bc.Wetness_pF(2.6));
		Weather w= cell.get_weather(day*0.5);



		//cell.get_rainfall().flux=10.0;
		//cout << "Rain: " << cell.get_transpiration().water_balance(day*0) << "m3/day" << endl;
		cout << cell.get_transpiration().flux_to(cell.get_layer(0),day*180.5);
		CVodeIntegrator integ(p,1e-6);
		while (integ.ModelTime()<day*1000)
		{
			integ.IntegrateUntil(integ.ModelTime() + math::day);
			cout << integ.ModelTime().ToString() << "P_u=" << cell.get_layer(0).get_potential() << " P_s=" << cell.get_layer(1).get_potential() << " d_s=" << cell.get_layer(1).get_thickness() << endl;
		}
		cin.get();
		

	}
#endif //Not def _DLL

