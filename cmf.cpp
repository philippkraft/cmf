

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

		timeseries ts(0*day,1*day,1);
		for (int i = 0; i < 10 ; ++i) ts.add(1.0);
		NeumannBoundary nbc(p,ts);
		p.debug=1;

 		Cell * c1=p.NewCell(0,0,0,100);
		VanGenuchtenMualem vgm;
		c1->add_variable_layer_pair(1.0,vgm);
		VariableLayerSaturated* sat= VariableLayerSaturated::get_from_cell(*c1);
		VariableLayerUnsaturated* unsat=&sat->UpperLayer();
		sat->set_potential(0.033);
		cout << sat->get_potential() << sat->get_upper_boundary() << endl;
		for (int i = 0; i < 100 ; ++i)
		{
			real sd=0.05 - i*0.05/50;
			c1->set_saturated_depth(sd);
			cout << " set:" << sd << " get(pot_s):" << c1->get_saturated_depth() << " boundary:" << sat->get_upper_boundary() << endl;
		}
// 		VarLayerPercolationSimple * perc=new VarLayerPercolationSimple(*unsat,*sat);
// 		cout << perc->q(*unsat,0*day);

// 		nbc.connect_to(c1->get_layer(1));
// 		Cell * c2=p.NewCell(0,10,0,100);
// 		c1->get_topology().AddNeighbor(*c2,10);
// 		c2->get_topology().AddNeighbor(*c1,10);
// 		c1->surfacewater_as_storage();
// 		c2->surfacewater_as_storage();
// 		connect_cells_with_flux(p.get_cells(),Manning::cell_connector);

	}
#endif //Not def _DLL

