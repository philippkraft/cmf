#ifndef __reach_network
#define __reach_network

#include <memory>


namespace cmf {
	namespace apps {

		enum SolverType
		{
			dense, banded, krylov, klu
		};
		int run_reaches(int levels, SolverType solver, int steps=0);

	}

}

#endif
