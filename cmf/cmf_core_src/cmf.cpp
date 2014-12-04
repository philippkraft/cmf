

// Copyright 2010 by Philipp Kraft
// This file is part of cmf.
//
//   cmf is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   cmf is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
//   


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
#include "math/Integrators/MultiIntegrator.h"
#include "Upslope/Soil/RetentionCurve.h"
#include "Atmosphere/Precipitation.h"
#include "water/flux_connection.h"
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
		
		
		project p="X Y";
		solute X=p.solutes[0];
		solute Y=p.solutes[1];
		timeseries ts(0*day,20*day,0);
		for (int i = 0; i < 100 ; ++i) ts.add(10 *((i+1) % 2));
		p.debug=1;
		Reach_ptr r=p.NewReach(Channel('R',10));
		NeumannBoundary::ptr nbc=NeumannBoundary::create(p,ts);
		VanGenuchtenMualem vgm(1.0,0.5,0.04,2);
		Cell* last_c=0;
		for (int i = 0; i <2; ++i)
		{
			Cell* c=p.NewCell(i,0,i*0.1,1);
			for (int i = 1; i < 11 ; ++i)
			{
				c->add_layer(i*0.1,vgm);
			}
			Richards::use_for_cell(*c);
			c->set_saturated_depth(0.5);
			//c->get_layer(0).set_wetness(0.5);
			if (last_c)
			{
				c->get_topology().AddNeighbor(*last_c,1);
				last_c->get_topology().AddNeighbor(*c,1);
			}
			PenmanMonteithET::use_for_cell(*c);
			last_c=c;
		}
		p.get_cell(-1).get_layer(-1)->conc(X,100);
		p.get_cell(-1).get_rainfall()->flux[0]=10.0;
		//p.get_cell(-1).get_rainfall().concentration[Y]=10.0;
		connect_cells_with_flux(p.get_cells(),Richards_lateral::cell_connector);
		//nbc.connect_to(p.get_cell(-1).get_layer(0));
 		CVodeIntegrator water_integ(p,1e-6);// water_integ.LinearSolver=0;
		water_integ.preconditioner='R';
		ImplicitEuler solute_integ(1e-6);
		SoluteWaterIntegrator integ(water_integ,solute_integ,p);
		//integ.MaxOrder=1;
		clock_t t=clock();
		real overflow=0.0,perc=0.0;
		int i=0;
		//cout << integ.ModelTime() << " dt:" << integ.TimeStep() << " perc=" << perc << " overflow=" << overflow << " s_pot=" << sat->get_potential() << " u_pot=" << unsat->get_potential() <<  endl;
		cout.precision(3);
		while (integ.get_t() < day*20)
		{
			integ.integrate_until(integ.get_t()+math::day);
			cout << integ.get_t();
			for (int i = 0; i < p.size() ; ++i)
			{
				if (i)
					cout << " q" << i << i-1 << "=" << p.get_cell(i).get_layer(-1)->flux_to(*p.get_cell(i-1).get_layer(-1),integ.get_t());
				//cout << "(" << p.get_cell(i).get_layer(0).get_state() << "," << p.get_cell(i).get_layer(-1).get_state() << ")";
				cout << "(X" << p.get_cell(i).get_layer(-1)->conc(X)<< ",Y"  << p.get_cell(i).get_layer(-1)->conc(Y) << ")";
			}
			cout << endl;
		}
		p.get_cell(0).remove_last_layer();

	}
#endif //Not def _DLL

