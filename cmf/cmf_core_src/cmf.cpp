

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
#include <chrono>
#include "Atmosphere/Meteorology.h"
#include "Upslope/Cell.h"
#include "Upslope/vegetation/ET.h"
#include "math/Integrators/BDF2.h"
#include "math/Integrators/cvode3.h"
#include "math/Integrators/WaterSoluteIntegrator.h"
#include "math/Integrators/RKFIntegrator.h"
#include "math/Integrators/MultiIntegrator.h"
#include "Upslope/Soil/RetentionCurve.h"
#include "Atmosphere/Precipitation.h"
#include "water/flux_connection.h"
#include "upslope/connections/subsurfacefluxes.h"
#include "upslope/connections/surfacefluxes.h"
#include "upslope/connections/infiltration.h"
#include "upslope/connections/Percolation.h"
#include "Upslope/vegetation/ShuttleworthWallace.h"
#include "Reach/reach.h"
#include "Reach/ManningConnection.h"
#include "Upslope/algorithm.h"

	/// Main function of the program. Only for debugging and testing, the real CMF<sub>lib</sub> will be compiled as a DLL and the 
	/// main function will be replaced by Python code

void create_hillslope(cmf::project& p, int length) {
	using namespace cmf::upslope;
	using namespace cmf::upslope::connections;
	using namespace cmf::upslope::ET;

	VanGenuchtenMualem vgm(1.0, 0.5, 0.04, 2);

	Cell* last_c = 0;
	for (int i = 0; i < length; ++i)
	{
		Cell* c = p.NewCell(i, 0, i*0.1, 1);
		for (int i = 1; i < 11; ++i)
		{
			c->add_layer(i*0.1, vgm);
		}
		cmf::upslope::connections::Richards::use_for_cell(*c);
		c->set_saturated_depth(0.5);
		//c->get_layer(0).set_wetness(0.5);
		if (last_c)
		{
			c->get_topology().AddNeighbor(*last_c, 1);
			last_c->get_topology().AddNeighbor(*c, 1);
		}
		PenmanMonteithET::use_for_cell(*c);
		last_c = c;
	}
	p.get_cell(-1).get_layer(-1)->conc(p.solutes[0], 100);
	p.get_cell(-1).set_rainfall(10.0);
	//p.get_cell(-1).get_rainfall().concentration[Y]=10.0;
	cell_vector cells = p.get_cells();
	connect_cells_with_flux(cells, Richards_lateral::cell_connector);

}

int64_t run(cmf::math::CVode3* integ) {
	auto tstart = std::chrono::high_resolution_clock::now();
	while (integ->get_t() < cmf::math::day * 20)
	{
		integ->integrate_until(integ->get_t() + cmf::math::day);
		std::cout << integ->get_t();
		/*/
		for (int i = 0; i < p.size() ; ++i)
		{
		if (i)
		cout << " q" << i << i-1 << "="
		<< p.get_cell(i).get_layer(-1)->flux_to(*p.get_cell(i-1).get_layer(-1),integ->get_t());
		//cout << "(" << p.get_cell(i).get_layer(0).get_state() << "," << p.get_cell(i).get_layer(-1).get_state() << ")";
		cout << "(X" << p.get_cell(i).get_layer(-1)->conc(X)<< ",Y"  << p.get_cell(i).get_layer(-1)->conc(Y) << ")";
		} */
		std::cout << std::endl;
	}
	auto tend = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart).count();
}

int main(int argc, char* argv[])
	{
		using namespace std;
		using namespace cmf;
		using namespace	cmf::math;
		using namespace cmf::water;
		
		
		project p="X Y";
		solute X=p.solutes[0];
		solute Y=p.solutes[1];
		timeseries ts(0*day,20*day,0);
		for (int i = 0; i < 100 ; ++i) 
			ts.add(10 *((i+1) % 2));

		create_hillslope(p, 20);
		
		CVode3 * integ = new CVodeKrylov(p, 1e-9, 10, 'L');

		int64_t duration = run(integ);
		cout.precision(3);

		std::ofstream outfile;
		outfile.open("performance.txt", std::ios_base::app);
		outfile << std::endl << integ->to_string() << std::endl << "------------------" << std::endl << std::endl;
		outfile << integ->get_info().to_string();
		outfile << "Duration: " << duration << " ms" << std::endl;
		outfile.close();
		
		delete integ;

	}
#endif //Not def _DLL

