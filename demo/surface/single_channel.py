import json
import time

import pandas as pd

import cmf
import typing

class River(cmf.project):
    @property
    def slope(self):
        """
        Gets the mean slope of the river
        """
        return (self.reaches[0].position.z - self.reaches[-1].position.z) / (self.reaches[0].position.x - self.reaches[-1].position.x)

    @property
    def length(self):
        """
        Get the total length of the river in m
        """
        return sum(r.length for r in self.reaches)

    @property
    def depth(self):
        """
        Get or set the depth of the river for each reach. Set to a scalar
        will make the depth equal for each reach, or set a sequence with a value for each reach
        """
        return [r.depth for r in self.reaches]

    @depth.setter
    def depth(self, value):
        try:
            len(value)
        except TypeError:
            value = [value] * len(self.reaches)
        for r, v in zip(self.reaches, value):
            r.depth = v

    def velocity(self, t: cmf.Time) -> typing.List[float]:
        """
        Calculate the velocity for each reach in m/s
        """
        area = lambda r: r.channel.get_flux_crossection(r.depth)
        return [
            up.flux_to(down, t) / area(up) / 86_400 if area(up)>0 else 0.0
            for up, down in zip(self.reaches[:-1], self.reaches[1:])
        ]

    def _make_reach(self, i, slope, reachlen):
        """
        Creates a reach at position i
        """
        channel = cmf.TriangularReach(reachlen)
        r = self.NewReach(i * reachlen, 0, - i * reachlen * slope, channel)
        r.Name = f'R{i:03}'
        return r

    def _make_boundaries(self, upper_boundary_flow):
        """
        Creates the upper and lower boundary condition
        """
        upper_bc = self.NewNeumannBoundary('inflow', self.reaches[0])
        upper_bc.flux = upper_boundary_flow
        out_x = sum(r.length for r in self.reaches)
        lower_bc = self.NewOutlet('outflow', out_x, 0, out_x * self.slope)
        return upper_bc, lower_bc

    def _connect(self, diffusive):
        """
        Connects the reaches and the lower boundary condition
        """
        for up, down in zip(self.reaches[:-1], self.reaches[1:]):
            up.set_downstream(down)
        down: cmf.Reach = self.reaches[-1]
        cmf.Manning_Kinematic(down, self.lower_bc, down.channel)

    def get_steadystate_depth(self, index=0):
        """
        Calculates the steady state depth for the reach at index according to its influx.
        Uses scipy's root finding function brentq
        """
        from scipy.optimize import brentq
        r = self.reaches[index]
        def f(d):
            r.depth = d
            return r(cmf.Time())
        return brentq(f, 0, 100)

    def __init__(self, n_reaches, totlength, slope, upper_boundary_flow, solutes='', diffusive=False):
        """
        Creates a project for a river with constant slope, no forks and only inflow at the upper end
        :param n_reaches: Number of numerical reaches
        :param totlength: Total length of the river in m
        :param slope: Constant slope of the river in m/m
        :param upper_boundary_flow: in flow from the upper boundary in m³/day
        :param solutes: A string denoting the names of solutes, if any
        :param diffusive: If True, use cmf.Manning_Diffusive for flow between the reaches. Outflow to boundary is always
        Kinematic
        """
        super().__init__(solutes)
        self.reachlen = totlength / n_reaches
        reaches = [
            self._make_reach(i, slope, self.reachlen)
            for i in range(n_reaches)
        ]
        self.upper_bc, self.lower_bc = self._make_boundaries(upper_boundary_flow)
        self._connect(diffusive)

def run_conc_step_function(p: River, solver, verbose=True, concentration=1):
    """
    Runs a concentration step function. River should be in steady state for water and
    the solute storage should be zero for all storages.

    First, the theoretical time for the solute to run to the outlet is calculated:

    .. math ::
        \\tau = \\frac{l}{v}

    The upper boundary condition's concentration is set to `concentration` (usually 1, except for scaling experiments).
    The model is run until :math:`2 \\tau` and saves the time, when [X]>p*C_0 as t_p, where p is [.01, .50, .99]
    If the level is never reach t_p is 2*tau. The difference
    between tau (t_theory), t_01 and t_99 is an indicator for numerical dispersion

    :param p: The river project
    :param solver: A solver
    :param verbose: If True, print to stdout during the model runtime
    :return: t_theory, t_01, t_50, t_99
    """
    X = p.solutes[0]
    p.upper_bc.concentration[X] = concentration
    def max_reach(threshold):
        return max([i for i, r in enumerate(p.reaches) if r[X].conc > threshold] or [-1])
    t_theory = cmf.sec * (p.length / p.velocity(solver.t)[-1])
    t_01 = t_50 = t_99 = cmf.Time()
    for t in solver.run(cmf.Time(), t_theory * 2, cmf.min):
        lbcX = p.lower_bc.conc(t, X)
        if lbcX < 0.01*concentration: t_01 = t
        if lbcX < 0.50*concentration: t_50 = t
        if lbcX < 0.99*concentration: t_99 = t
        if verbose:
            print(f'{t:%H:%M:%S} : R{max_reach(.01)}[X]>.01, R{max_reach(.99)}[X]>.99')
    return t_theory, t_01, t_50, t_99


class Result(dict):
    """
    Creates a result dictionary from the current state of the solver
    :param solver:
    :param kwargs:
    :return:
    """
    def __init__(self, p: cmf.project, solver: cmf.CVodeBase, **kwargs):
        t = solver.t
        info = solver.get_info()
        self.update(
            dict(
                n_reaches=len(p.reaches),
                length=p.length,
                slope=p.slope,
                inflow=p.upper_bc(t),
                solutes= ', '.join(f'{s}' for s in p.solutes),
                t=t / cmf.sec,
                solver=solver.to_string(),
                open_mp_threads=cmf.get_parallel_threads()
            ) | {
                k: getattr(info, k)
                for k in dir(info)
                if not any(k.startswith(pre) for pre in ('this', 'to_', '_', 'sundials'))
            } | kwargs
        )

    def __str__(self):
        return '\n'.join(f'{k}: {v}' for k, v in result.items())


def main(n_reaches, totlength, slope, upper_boundary_flow, solutes='', diffusive=False, verbose=False):
    # Make model
    p = River(n_reaches, totlength, slope, upper_boundary_flow, solutes=solutes, diffusive=diffusive)
    # Bring in steady state (with empty solute storages)
    p.depth = p.get_steadystate_depth()
    # Create frst solver
    solver = cmf.CVodeKLU(p)
    # Run to ensure the steady state
    solver(cmf.h)
    wallclock_start = time.time()
    cpu_start = time.process_time()
    # Make a second solver
    solver = cmf.CVodeKLU(p)
    # Run the step function
    t_theory, t_01, t_50, t_99 = run_conc_step_function(p, solver, verbose=verbose)
    return Result(
        p, solver,
        step_t_theory=t_theory/cmf.sec, step_t_01=t_01/cmf.sec, step_t_50=t_50/cmf.sec, step_t_99=t_99/cmf.sec,
        wallclock_time=time.process_time() - wallclock_start,
        cpu_time=time.process_time() - cpu_start
    )



if __name__ == '__main__':
    l_per_s = 86.4  #  m³/day
    cmf.set_parallel_threads(1)

    for i in range(1, 10):
        result = main(2 ** i, 1024, 0.001, 10 * l_per_s, solutes='X')
        print(2 ** i)
        pd.DataFrame([result], index=[i]).to_hdf('data/single_channel.h5', 'table', append=True)
    df = pd.read_hdf('data/single_channel.h5', 'table')
    print(df)