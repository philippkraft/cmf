
// Include a little bit of everything for everybody
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cmath>
#include "../project.h"
#include "../atmosphere/Meteorology.h"
#include "../Upslope/Cell.h"
#include "../Upslope/vegetation/ET.h"
#include "../math/integrators/cvodeIntegrator.h"
#include "../math/integrators/explicit_euler.h"
#include "../Upslope/Soil/RetentionCurve.h"
#include "../Upslope/connections/subsurfacefluxes.h"
#include "../Upslope/connections/infiltration.h"
#include "../Upslope/connections/percolation.h"
#include "../Reach/ManningConnection.h"
#include "../water/WaterStorage.h"
#include "../water/simple_connections.h"

/// Main function of the program. Only for debugging and testing, the real CMF<sub>lib</sub> will be compiled as a DLL and the 
/// main function will be replaced by Python code
#include <ctime>
#include <cmath>
/// Prints calculation time measurements
void printTime(const std::string& text,clock_t lastClock)
{
	std::cout << text << ((double)(clock()-lastClock))/CLOCKS_PER_SEC << " s" << std::endl;
}
using namespace std;
using namespace cmf;
using namespace	cmf::atmosphere;
using namespace	cmf::math;
using namespace cmf::water;
using namespace cmf::upslope;
using namespace cmf::upslope::connections;
using namespace cmf::upslope::ET;
using namespace cmf::river;
using namespace cmf::geometry;



// Loads cells from the file maimai.cells and creates the topological relation
// from the file maimai.topology
int load_cells(project& p,string name) {
	// Get filename for the cells
	string filename = name + ".cells";
	ifstream cellfile,topofile;
	try {
		cellfile.open(filename.c_str());
	} catch (ifstream::failure e) {
		cerr << "Could not open " << filename << ", does it exist?" << endl;
		return 0;
	}
	double x,y,z,area,w;
	int id, n_id;
	// Get location (x,y,z) and area for each cell from the file
	while (!cellfile.eof()) {
		cellfile >> id >> x >> y >> z >> area;
		Cell* c = p.NewCell(x,y,z,area);
		c->Id = id;
	}
	cellfile.close();
	// Get topological relation (adjacent cells with length of shared boundary) from fuke
	filename = name + ".topology";
	topofile.open(filename.c_str());
	while(!topofile.eof()) {
		topofile >> id >> n_id >> w;
		p.get_cell(id).get_topology().AddNeighbor(p.get_cell(n_id),w);
	}
	return 1;
}

// Adds storages and local connections to a cell
void designcell(Cell& c) {
	// Lower boundary for layers (in m)
	double depth[] = {0.05,0.1,0.2,0.3,0.5,0.75,1.0,1.3,1.7,2.};
	// Create a surfacewater storage
	c.surfacewater_as_storage();
	// Add layers with changing sat. conductivity to the cell
	for (int i = 0; i < 10 ; ++i)	{
		VanGenuchtenMualem vgm(15 * exp(-depth[i]));
		c.add_layer(depth[i],vgm);
	}
	// Use Richards equation for this cell
	Richards::use_for_cell(c);
	// Use MatrixInfiltration for this cell
	MatrixInfiltration::use_for_cell(c);
	// Use PenmanMonteithET for evapotranspiration
	PenmanMonteithET::use_for_cell(c);
	// Initialize water content of layers (hydrostatic equilibrium with 
	// groundwater depth at 1m below ground
	c.set_saturated_depth(c.z*.5);
	return;
}
// Loads the meteorological data from binary saved timeseries
void load_meteo(project& p,std::string name) {
	// Create meteorological station
	MeteoStation::ptr meteo = p.meteo_stations.add_station(name,point(0,0,0));
	// Load daily max temp (in degC)
	meteo->Tmax = timeseries::from_file(name+".Tmax");
	// Load daily min temp (in degC)
	meteo->Tmin = timeseries::from_file(name+".Tmin");
	// Load mean humidity (in %)
	meteo->rHmean = timeseries::from_file(name+".rHmean");
	// load windspeed (in m/s)
	meteo->Windspeed = timeseries::from_file(name+".Windspeed");
	// load sunshine fraction (in h/h)
	meteo->Sunshine = timeseries::from_file(name+".Sunshine");
	// load rainfall (in mm/day)
	timeseries rain = timeseries::from_file(name + ".rain");
	// Add rainfall station to project
	p.rainfall_stations.add(name,rain,point(0,0,0));
	// Attach all cells to meteo station
	p.use_nearest_meteo();
	// Attach all cells to rainfall station
	p.use_nearest_rainfall();
	return;
}

// Initializes all soil water storages with a solute conc. of 1.
// Can be used to simulate a wash out process
void init_solute(project & p,solute X) {
	for (int i = 0; i < p.size() ; ++i)	{
		Cell& c=p[i];
		for (int j = 0; j < c.layer_count() ; ++j)	{
			SoilLayer::ptr l=c.get_layer(j);
			l->conc(X,1.0);
		}
	}
}
int create3d(std::string name) {
	// If you want to simulate solute transport (wash out of old water), compile with SOLUTE peprocessor variable
#ifdef SOLUTE
	project p="X";
	solute X=p.solutes[0];
#else
	project p;
#endif
	cout << "load cells...";
	if (!load_cells(p,name)) {
		return 0;
	};
	cout << p.size() << endl;
	cout << "Design cells";
	for (int i = 0; i < p.size() ; ++i)	
		designcell(p.get_cell(i));
	cout << endl << "Connect cells";
	cmf::upslope::cell_vector cells = p.get_cells();
	connect_cells_with_flux(cells,Richards_lateral::cell_connector);
	connect_cells_with_flux(cells,Manning_Kinematic::cell_connector);
	cout << endl << "Make outlet";
	Cell& outcell = p.get_cells().get_lowest();
	DirichletBoundary::ptr outlet = p.NewOutlet("outlet",outcell.x-10,outcell.y,outcell.z-.3);
	Manning_Kinematic* outletflow = new Manning_Kinematic(OpenWaterStorage::cast(outcell.get_surfacewater()) ,outlet, Channel('R',10,10));
	cout << endl << "load meteo";
	load_meteo(p,"giessen");
#ifdef SOLUTE
	cout << endl << "Initialize soil water concentration";
	init_solute(p,X);
#endif
	cout << endl << "make solver";

	CVodeIntegrator integ(p,1e-6);
	Time start(1,1,1980);
	integ.set_t(start);

	time_t walltime=time(0),walltimestart=time(0);
	real overflow=0.0,perc=0.0;
	int i=0;
	cout.precision(3);
	cout << endl << "run" << endl;
	double area = p.get_cells().get_area();
	while (integ.get_t() < start + year)
	{
		Time t = integ.get_t();
		integ.integrate_until(t+math::h);
		cout << integ.get_t().AsDate();
		cout << " q=" << outletflow->q(*outlet,t)<< "m3/day";
#ifdef SOLUTE
		cout << " [X]q="<< outletflow->conc(t,X);
#else
		cout << " d_sat[out]=" << outcell.get_saturated_depth() << "m";
#endif
		cout << " P=" << p.rainfall_stations[0]->data.get_t(t);
		cout << endl;
	}
	return 0;

}
int createsimple() {
#ifdef SOLUTE
	project p="X";
	solute X=p.solutes[0];
#else
	project p;
#endif
	p.debug = true;
	WaterStorage::ptr w1 = p.NewStorage("w1",0,0,0);
	WaterStorage::ptr w2 = p.NewStorage("w2",10,0,0);
	WaterStorage::ptr w3 = p.NewStorage("w3",20,0,0);
	w1->set_volume(1.0);
	kinematic_wave* con1 = new kinematic_wave(w1,w2,1.0);
	kinematic_wave* con2 = new kinematic_wave(w2,w3,1.0);
	CVodeIntegrator integ(p);
	Time t0 = Time(1,1,2000);
	integ.set_t(t0);
	cout << "Integrator created with " << integ.size() << " states" << endl;
	while (integ.get_t()<Time(4,1,2000)) {
		integ.integrate_until(integ.get_t()+math::h);
		Time t = integ.get_t();
		cout << t.AsDate().to_string() << " V1m=" << w1->get_volume() << " V2m=" << w2->get_volume() << endl; 
		
	}
	return 0;

}
int main(int argc,const char* argv[])
{
	if (argc>=2) {
		string name=argv[1];
		create3d(name);
	} else {
		createsimple();
	}
	cin.get();
}
