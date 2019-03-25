
#include <vector>
#include <string>
#include <algorithm>

#include <chrono>

#include <iostream>
#include <iomanip>

#include "../project.h"
#include "../reach/ReachType.h"
#include "../reach/Reach.h"
#include "../water/boundary_condition.h"

#include "../math/integrators/cvode.h"
#include "../math/time.h"

namespace cmf {
	namespace apps {
		enum SolverType
		{
			dense, banded, krylov, klu
		};
		class ReachWithSource {
		public:
			cmf::river::Reach::ptr r;
			cmf::water::NeumannBoundary::ptr s;

			ReachWithSource(cmf::project& p, 
				double x, double y, double z, double l, 
				std::string Name, bool diffusive)
			{
				cmf::river::TriangularReach rtype(l);
				r = p.NewReach(x, y, z, rtype, diffusive);
				r->Name = Name;
				s = cmf::water::NeumannBoundary::create(r);
			}
		};

		class ReachNetwork {
		public:
			cmf::project p;
			ReachWithSource root;
			cmf::water::DirichletBoundary::ptr outlet;
			bool diffusive;
			double reach_length;
			typedef std::vector<ReachWithSource> river_vec;
			river_vec reaches;

			ReachNetwork(double length, bool _diffusive)
				: 
				diffusive(_diffusive), 
				reach_length(length),
				root(p, 0, 0, 0, length, "x", _diffusive)
			{
				outlet = p.NewOutlet("o", 0, -100, -1);
				root.r->set_outlet(outlet);				
			}
			size_t add_reaches(int max_level) {
				// river_vec root_reach = { root };
				river_vec upstream = make_reaches(1, max_level, { root });
				reaches.insert(reaches.end(), upstream.begin(), upstream.end());
				reaches.push_back(root);
				return reaches.size();
			}

			river_vec make_reaches(int level, int max_level, const river_vec& downstream) {
				river_vec upstream;
				double x_offset = reach_length / (level * level * 2);
				if (level <= max_level) {
					for (auto& rd : downstream) {
						ReachWithSource u1(p, 
							rd.r->position.x - x_offset, 
							level * reach_length, 
							level * 1.0, 
							reach_length, 
							rd.r->Name + "r", diffusive);
						u1.r->set_downstream(rd.r);
						upstream.push_back(u1);
						ReachWithSource u2(p, 
							rd.r->position.x + x_offset, 
							level * reach_length, 
							level * 1.0, 
							reach_length,
							rd.r->Name + "l", diffusive);
						u2.r->set_downstream(rd.r);
						upstream.push_back(u2);
					}
					river_vec further_up = make_reaches(level + 1, max_level, upstream);
					further_up.insert(further_up.end(), upstream.begin(), upstream.end());
					return further_up;
				}
				return upstream;
			}
			size_t size() const
			{
				return reaches.size();
			}
			
			void set_inflow(double total_flow)
			{
				double inflow = total_flow / size();
				for (auto& r : reaches) {
					r.s->set_flux(inflow);
				}
			}
			void set_initial_depth(double depth_in_m) {
				for (auto& r : reaches) {
					r.r->set_depth(depth_in_m);
				}

			}
			double get_inflow() {
				double sum = 0.0;
				for (auto& r : reaches) {
					sum += r.s->get_flux()[-1];
				}
				return sum;
			}
			double get_outflow(cmf::math::Time t) {
				return this->outlet->waterbalance(t);
			}
		};




		std::unique_ptr<cmf::math::CVodeBase> make_solver(cmf::project p, SolverType _solver) {
			using namespace cmf::math; 
			typedef std::unique_ptr<CVodeBase> solver_type;
			switch (_solver) {

			case dense:
				return solver_type(new CVodeDense(p));
			case banded:
				return solver_type(new CVodeBanded(p));
			case klu:
				return solver_type(new CVodeKLU(p));
			case krylov:
				return solver_type(new CVodeKrylov(p));
			default:
				throw std::runtime_error("Unknown solver type");
			}

		}
		int run_reaches(int levels, SolverType _solver, int steps)
		{
			std::cout << std::setprecision(8);
			// std::cout << "Create network\n";
			ReachNetwork network(100, false);
			try {
				// std::cout << ".. add reaches\n";
				network.add_reaches(levels);
			} catch (const std::runtime_error& e) {
				std::cerr << e.what() << "\n";
				exit(1);
			}
			// std::cout << ".. network has " << network.size() << " reaches\n";
			network.set_inflow(100);
			network.set_initial_depth(0.1);
			// std::cout << ".. total inflow = " << network.get_inflow() << " m�/day\n\n";

			// std::cout << "Creating solver\n";

			auto solver = make_solver(network.p, _solver);
			/*
			std::cout 
				<< ".." << solver->to_string() << " has " << solver->size() << " states\n"
				<< "..initialize";
			solver->initialize();
			std::cout
				<< " ok\n"
				<< "\nRun\n--------\n";
			*/

			auto tstart = std::chrono::high_resolution_clock::now();
			cmf::math::Time duration = cmf::math::day;
			try {
				if (steps) {
					for (int i = 0; i<steps; ++i)
						solver->integrate(duration, duration);
				}
				else {
					solver->integrate_until(duration);
				}

			}
			catch (const std::runtime_error& e) {
				std::cout << solver->get_t().to_string() << "\n";
				std::cout << solver->get_info().to_string();
				std::cerr << e.what() << "\n";
				exit(1);
			}
			/*
			std::cout 
				<< "Q(o) =" << network.get_outflow(solver->get_t()) << " m�/day outflow\n"
				<< "d(Rx)=" << network.root.r->get_depth() * 100 << " cm depth at root\n"
				<< "T    =" << solver->get_t().to_string() << "\n\n"
				<< solver->get_info().to_string() << "\n"
				<< "Get Jacobian\n";
			
			cmf::math::num_array jac = solver->_get_jacobian();
			size_t N = solver->size();
			std::cout
				<< "    J[R0,C0]=" << jac[0] << "\n"
				<< "    J[R0,C1]=" << jac[N] << "\n"
				<< "    J[R0,C2]=" << jac[2 * N] << "\n"
				<< "    J[RN,CN]=" << jac[jac.size() - 1] << "\n";
			*/
			auto tend = std::chrono::high_resolution_clock::now();
			int64_t wall_clock_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart).count();

			auto info = solver->get_info();
			std::cout 
				<< solver->to_string() << "\t" 
				<< info.size << "\t" 
				<< info.dxdt_method_calls << "\t" 
				<< wall_clock_ms * 1e-3 << " s \n";
			return 0;

		}

	}
}

int main(int argc, char* argv[])
{
	for (int i=1; i<=10; ++i)
		cmf::apps::run_reaches(i, cmf::apps::dense, 0);
	std::cin.ignore();

}
