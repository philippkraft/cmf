#include "nash_cascade.h"

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <chrono>
#include "../math/time.h"
#include "../math/timeseries.h"
#include "../project.h"
#include "../water/WaterStorage.h"
#include "../water/boundary_condition.h"
#include "../water/simple_connections.h"
#include "../math/integrators/RKFintegrator.h"
#include "../math/integrators/implicit_euler.h"



namespace cmf {
	
	namespace apps {

		class NashBoxModel {
		
		public:

			std::vector<cmf::water::WaterStorage::ptr> storages;
			project p;
			cmf::water::DirichletBoundary::ptr outlet;

			NashBoxModel(int size, double residence_time)
			{
				outlet = p.NewOutlet("out", 0, 0, 0);
				
				if (size < 1) {
					throw std::runtime_error("Size of model may not be smaller than 1");
				}
				for (int i = 1; i <= size; ++i) {
					std::string name("S");
					name += std::to_string(i);
					auto stor = p.NewStorage(name, i, 0, i);
					if (i > 1) {
						new cmf::water::LinearStorageConnection(stor, storages.back(), residence_time / size);
					}
					else {
						new cmf::water::LinearStorageConnection(stor, outlet, residence_time / size);
					}
					storages.push_back(stor);
				}
			}

			cmf::math::timeseries run(size_t nsteps, cmf::math::Time dt = cmf::math::h, double start_vol = 1.0) {
				cmf::math::ImplicitEuler solver(this->p);
				for (auto s : this->storages) {
					s->set_state(0.0);
				}
				this->storages.back()->set_volume(start_vol);

				cmf::math::timeseries res(solver.get_t(), dt);

				res.add(this->outlet->waterbalance(solver.get_t()));

				for (int i = 0; i < nsteps; ++i) {
					solver.integrate_until(solver.get_t() + dt, dt);
					res.add(this->outlet->waterbalance(solver.get_t()));
				}
				return res;
			}

		};


		int run_nash_cascade(int size)
		{
			NashBoxModel m(size, 1.0);
			auto res = m.run(24);

			std::cout
				<< "Run for 24h\n" << "----------------\n"
				<< res.end().to_string() << ":"
				<< res[-1] << " m³/day"
			;
			return 0;
		}

	}
}
