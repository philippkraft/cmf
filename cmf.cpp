

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
#include "math/Integrators/WaterSoluteIntegrator.h"
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
		VanGenuchtenMualem vgm(2.0,0.04,0.5,1);
		Cell* last_c=0;
		for (int i = 0; i < 10 ; ++i)
		{
			Cell* c=p.NewCell(i,0,i*0.5,1);
			c->add_layer(1,vgm);
			c->get_layer(0).set_wetness(0.5);
			MatrixInfiltration::use_for_cell(*c);
			if (last_c)
			{
				c->get_topology().AddNeighbor(*last_c,1);
				last_c->get_topology().AddNeighbor(*c,1);
			}
			last_c=c;
		}
		connect_cells_with_flux(p.get_cells(),Darcy::cell_connector);
		nbc.connect_to(p.get_cell(9).get_layer(0));
 		CVodeIntegrator water_integ(1e-6);water_integ.LinearSolver=0;
		BDF2 solute_integ(1e-6);
		SoluteWaterIntegrator integ(water_integ,solute_integ,p);
		p.get_cell(-1).get_layer(0).conc(X,1e2);
		//integ.MaxOrder=1;
		clock_t t=clock();
		real overflow=0.0,perc=0.0;
		int i=0;
		//cout << integ.ModelTime() << " dt:" << integ.TimeStep() << " perc=" << perc << " overflow=" << overflow << " s_pot=" << sat->get_potential() << " u_pot=" << unsat->get_potential() <<  endl;
		cout.precision(3);
		cout << p.get_cell(0).get_layer(0).Connection(p.get_cell(1).get_layer(0))->conc(h*0)[X] << endl;
		while (integ.ModelTime() < day*20)
		{
			integ.IntegrateUntil(integ.ModelTime()+math::h);
			cout << integ.ModelTime();
			for (int i = 0; i < p.size() ; ++i)
			{
				cout << "("<< p.get_cell(i).get_layer(0).get_wetness() << "," << p.get_cell(i).get_layer(0).conc(X) << ")";
			}
			cout << endl;
			//overflow=p.get_cell(0).get_surfacewater().water_balance(integ.ModelTime());
			//if (i++ % 100==0)
			//cout << integ.ModelTime() << " dt:" << integ.TimeStep() << " perc=" << perc << " overflow=" << overflow << " s_pot=" << sat->get_potential() << " u_pot=" << unsat->get_potential() <<  endl;
			//cout << "        u_Age=" << unsat->conc(X) << " s_Age=" << sat->conc(X) << endl;
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

