

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
#include "math/Integrators/FixpointImplicitEuler.h"
#include "math/Integrators/ExplicitEuler_fixed.h"
#include "math/Integrators/RKFIntegrator.h"
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
#include "Upslope/VarLayerPair.h"
#include "Upslope/connections/VarLayerPercolation.h"

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
		
		const Solute& X=Solutes::all().add("X");
		project p;

		timeseries ts(0*day,20*day,0);
		for (int i = 0; i < 100 ; ++i) ts.add(10 *((i+1) % 2));
		NeumannBoundary nbc(p,ts);
		p.debug=1;

 		Cell * c1=p.NewCell(0,0,0,1000);
		VanGenuchtenMualem vgm(2.0,0.04,0.5,1);
		c1->add_variable_layer_pair(1.0,vgm);
		VariableLayerSaturated* sat= VariableLayerSaturated::get_from_cell(*c1);
		VariableLayerUnsaturated* unsat=&sat->UpperLayer();
		//new VarLayerPercolationSimple(*unsat,*sat);
		new MatrixInfiltration(*unsat,c1->get_surfacewater());
		//MatrixInfiltration::use_for_cell(*c1);
		nbc.connect_to(*sat);
		c1->set_saturated_depth(0.5);
		//unsat->set_potential(-0.99);
		//sat->set_potential(-0.07);
		cout << sat->get_potential() << sat->get_upper_boundary() << endl;
		c1->get_rainfall().flux=timeseries(day*0,day*20,0);
		for (int i = 0; i < 100 ; ++i)
		{
			c1->get_rainfall().flux.add(0*(i % 2));
 		}
		PenmanMonteithET::use_for_cell(*c1);
		for (int i = 0; i < c1->layer_count() ; ++i)
		{
			c1->get_layer(i).Solute(X).AdditionalFlux=1.0;
		}
 		CVodeIntegrator integ(p,1e-6);integ.LinearSolver=0;
		//BDF2 integ(p,1e-4);
		//integ.MaxOrder=1;
		cout << unsat->flux_to(*sat,integ.ModelTime()) << endl;
		clock_t t=clock();
		real overflow=0.0,perc=0.0;
		int i=0;
		cout << integ.ModelTime() << " dt:" << integ.TimeStep() << " perc=" << perc << " overflow=" << overflow << " s_pot=" << sat->get_potential() << " u_pot=" << unsat->get_potential() <<  endl;
		
		while (integ.ModelTime() < day*200)
		{
			integ.IntegrateUntil(integ.ModelTime()+day);
			overflow=c1->get_surfacewater().water_balance(integ.ModelTime());
			perc=unsat->flux_to(*sat,integ.ModelTime());
			//if (i++ % 100==0)
			cout << integ.ModelTime() << " dt:" << integ.TimeStep() << " perc=" << perc << " overflow=" << overflow << " s_pot=" << sat->get_potential() << " u_pot=" << unsat->get_potential() <<  endl;
			cout << "        u_Age=" << unsat->conc(X) << " s_Age=" << sat->conc(X) << endl;
		}
// 		for (int i = 0; i < 100 ; ++i)
// 		{
// 			real sd=0.05 - i*0.05/50;
// 			c1->set_saturated_depth(sd);
// 			cout << " set:" << sd << " get(pot_s):" << c1->get_saturated_depth() << " boundary:" << sat->get_upper_boundary() << endl;
// 		}
// 
// 		nbc.connect_to(c1->get_layer(1));
// 		Cell * c2=p.NewCell(0,10,0,100);
// 		c1->get_topology().AddNeighbor(*c2,10);
// 		c2->get_topology().AddNeighbor(*c1,10);
// 		c1->surfacewater_as_storage();
// 		c2->surfacewater_as_storage();
// 		connect_cells_with_flux(p.get_cells(),Manning::cell_connector);

	}
#endif //Not def _DLL

