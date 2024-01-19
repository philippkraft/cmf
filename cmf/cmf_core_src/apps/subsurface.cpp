// Include a little bit of everything for everybody


#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <chrono>
#include "../project.h"
#include "../atmosphere/meteorology.h"
#include "../upslope/cell.h"
#include "../upslope/vegetation/ET.h"
#include "../math/integrators/cvode.h"
#include "../upslope/Soil/RetentionCurve.h"
#include "../atmosphere/precipitation.h"
#include "../water/flux_connection.h"
#include "../upslope/connections/subsurfacefluxes.h"
#include "../upslope/connections/surfacefluxes.h"
#include "../upslope/connections/infiltration.h"
#include "../upslope/connections/Percolation.h"
#include "../upslope/vegetation/ShuttleworthWallace.h"
#include "../reach/Reach.h"
#include "../reach/ManningConnection.h"
#include "../upslope/algorithm.h"

/// Main function of the program. Only for debugging and testing, the real CMF<sub>lib</sub> will be compiled as a DLL and the 
/// main function will be replaced by Python code

void create_hillslope(cmf::project& p, int length, std::string& name) {
	using namespace cmf::upslope;
	using namespace cmf::upslope::connections;
	using namespace cmf::upslope::ET;

	VanGenuchtenMualem vgm(1.0, 0.5, 0.04, 2);
	name = "Hillslope with " + std::to_string(length) + " cells";
	Cell* last_c = 0;
	for (int i = 0; i < length; ++i)
	{
		Cell* c = p.NewCell(i, 0, i*0.1, 1, true);
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
	cmf::atmosphere::ConstantRainSource::ptr
		rs(new cmf::atmosphere::ConstantRainSource(p, cmf::geometry::point(0, 0, 0), 10.0));

	rs->set_conc(p.solutes[1], 10.0);
	p.get_cell(-1).set_rain_source(rs);

	cell_vector cells = p.get_cells();
	connect_cells_with_flux(cells, Richards_lateral::cell_connector);
	connect_cells_with_flux(cells, KinematicSurfaceRunoff::cell_connector);

}

void create_3d(cmf::project& p, int size_x, int size_y, std::string& name) {

	using namespace cmf::upslope;
	using namespace cmf::upslope::connections;
	using namespace cmf::upslope::ET;

	VanGenuchtenMualem vgm(10.0, 0.5, 0.04, 2);
	name = "3d Raster with " + std::to_string(size_x) + "*" + std::to_string(size_y) + " cells";

	for (int iy = 0; iy < size_x; ++iy)
	{
		for (int ix = 0; ix < size_x; ++ix) {
			Cell * c = p.NewCell(ix, iy, ix + iy * 0.1, 1, true);
			for (int i = 1; i < 11; ++i) {
				c->add_layer(i*0.1, vgm);
			}

			cmf::upslope::connections::Richards::use_for_cell(*c);
			c->set_saturated_depth(0.5);

			if (ix > 0) {
				cmf::upslope::Cell& other = p.get_cell(iy * size_x + (ix - 1));
				c->get_topology().AddNeighbor(other, 1);
				other.get_topology().AddNeighbor(*c, 1);
			}

			if (iy > 0) {
				cmf::upslope::Cell& other = p.get_cell((iy - 1) * size_x + ix);
				c->get_topology().AddNeighbor(other, 1);
				other.get_topology().AddNeighbor(*c, 1);
			}

			p.get_cell(-1).get_layer(-1)->conc(p.solutes[0], 100);

			cmf::atmosphere::ConstantRainSource::ptr
				rs(new cmf::atmosphere::ConstantRainSource(p, cmf::geometry::point(0, 0, 0), 10.0));

			rs->set_conc(p.solutes[1], 10.0);
			p.get_cell(-1).set_rain_source(rs);
		}
	}
}

int64_t run(cmf::math::CVodeBase* integ, cmf::project& p) {
	auto tstart = std::chrono::high_resolution_clock::now();
	std::cout.precision(3);
	while (integ->get_t() < cmf::math::day * 20)
	{
		integ->integrate_until(integ->get_t() + cmf::math::h);
		std::cout << integ->get_t()
			<< p.get_cell(0).get_saturated_depth() << " m below ground";
		std::cout << std::endl;
	}
	auto tend = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart).count();
}

int main(int argc, char* argv[]) {
	using namespace std;
	using namespace cmf;
	using namespace	cmf::math;
	using namespace cmf::water;

	project p("X Y");
	solute X = p.solutes[0];
	solute Y = p.solutes[1];
	timeseries ts(0 * day, 20 * day, 0);
	for (int i = 0; i < 100; ++i)
		ts.add(10 * ((i + 1) % 2));
	std::string name;
	create_3d(p, 10, 10, name);
	cmf::math::state_list s = p;
	CVodeBase * integ = new CVodeKLU(s, 1e-9);

	int64_t duration = run(integ, p);

	std::ofstream outfile;
	outfile.open("performance.txt", std::ios_base::app);
	outfile << std::endl;
	outfile << name << ": ";
	outfile << integ->to_string() << std::endl << "------------------" << std::endl << std::endl;
	outfile << integ->get_info().to_string();
	outfile << "Duration: " << duration << " ms" << std::endl;
	outfile.close();

	delete integ;
	return 0;

}
