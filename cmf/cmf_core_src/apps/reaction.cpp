//
// Created by kraft-p on 28.08.2019.
//
#include <iostream>

#include "../project.h"
#include "../water/WaterStorage.h"
#include "../water/SoluteStorage.h"
#include "../math/integrators/implicit_euler.h"
#include "../math/time.h"

using namespace cmf;
using namespace cmf::water;
using namespace cmf::math;

class ReactionProject: public cmf::project {
public:
    WaterStorage::ptr wstor;
    ReactionProject():
        project("A B C"),
        wstor(this->NewStorage("wstor"))
    {

    }
};

int main() {
    ReactionProject rp;
    solute A(rp.solutes[0]), B(rp.solutes[1]), C(rp.solutes[2]);
    auto& wstor = *rp.wstor;
    wstor.set_volume(1.0);
    auto r_A = std::make_shared<SoluteConstantFluxReaction>(1.0);
    auto r_AB = std::make_shared<SoluteEquilibriumReaction>(A, B, 1, 5);
    auto r_BC = std::make_shared<Solute1stOrderReaction>(B, C, 0.5);
    auto r_C = std::make_shared<SoluteDecayReaction>(2);

    wstor[A].reactions = {r_A, r_AB};
    wstor[B].reactions = {r_AB, r_BC};
    wstor[C].reactions = {r_BC, r_C};

    auto solver = ImplicitEuler(rp);

    while (solver.get_t() < day * 30) {
        solver.integrate(solver.get_t() + h, h);
        std::cout << solver.get_t().to_string() << ": ";
        for (const auto& X: rp.solutes) {
            std::cout << " [" << X.Name << "]=" << wstor[X].get_conc();
        }
        std::cout << "\n";
    }

}