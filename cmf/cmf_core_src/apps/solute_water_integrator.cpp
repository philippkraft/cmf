//
// Created by kraft-p on 08.07.2019.
//
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

#include "project.h"
#include "water/WaterStorage.h"
#include "water/simple_connections.h"
#include "math/integrators/explicit_euler.h"
#include "math/integrators/WaterSoluteIntegrator.h"


int main() {
    using namespace cmf;
    using namespace cmf::water;
    using namespace cmf::math;

    project p("X Y");
    auto& X=p.solutes[0];
    auto& Y=p.solutes[1];
    std::vector<WaterStorage::ptr> storages;
    int max_stor = 10;
    for(int i=0;i<max_stor;++i) {
        storages.push_back(
                p.NewStorage(std::string("W") + std::to_string(i))
                );
        if (i) {
            new cmf::water::LinearStorageConnection(storages[i - 1], storages[i], 1.0);
        }
    }
    WaterStorage& W0 = *storages[0];
    W0.set_volume(1.0);
    W0[X].set_conc(1.0);
    W0[Y].set_conc(0.5);

    auto w_solver_template=new HeunIntegrator();
    auto s_solver_template= new ExplicitEuler_fixed();
    state_list sl(p);
    SoluteWaterIntegrator solver(p.solutes, *w_solver_template, *s_solver_template, sl);
    delete w_solver_template;
    delete s_solver_template;
    solver.integrate(h, h);
    return 0;
}