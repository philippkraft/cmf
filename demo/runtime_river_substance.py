
import cmf
import numpy as np
from datetime import datetime,timedelta
from collections import Counter
import time
import timeit
import argparse

def test(d, withsubstance=False, threads=1):
    """
    creates a set of river segments with and without substance. the solver
    can be set up with a varying nu,ber of threads as well as separated for
    water and substance fluxes.
    """
    cmf.set_parallel_threads(threads)
    
    if withsubstance:
        p = cmf.project("X")
        X, = p.solutes
    else:
        p = cmf.project()
        X = None
    # Create a triangular reach crosssection for 10 m long reaches with a bankslope of 2
    shape = cmf.TriangularReach(10.,2.)
    # Create a 1km river with 100 reaches along the x axis and a constant slope of 1%
    reaches = [p.NewReach(i,0,i*.01,shape,False) for i in range(0,1000,d)]
    for r_lower, r_upper in zip(reaches[:-1],reaches[1:]):
        r_upper.set_downstream(r_lower)
    for i, r in enumerate(reaches):
        r.Name = 'R{}'.format(i+1)
        
    # Initial condition: 10 cmf of water in the most upper reach
    for r in reaches:
        r.depth=0.1
        if withsubstance:
            r.conc(X, 1.0)

            # Template for the water solver
    wsolver = cmf.CVodeIntegrator(1e-9)
    # Template for the solute solver
    ssolver = cmf.HeunIntegrator()
    # Creating the SWI, the storage objects of the project are internally assigned to the correct solver
    solver = cmf.SoluteWaterIntegrator(p.solutes, wsolver, ssolver, p)
    # solver = cmf.CVodeIntegrator(p,1e-6)
    assert solver.size() == len(reaches) * (len(p.solutes) + 1)

    # We store the results in this list
    depth = [[r.depth for r in reaches]]

    start = time.time()
    c = Counter()
    for t in solver.run(datetime(2012,1,1),datetime(2012,1,2), cmf.min * 10):
        # c.update([solver.get_error().argmax()])
        print('{:10.3} sec, {}, {}, {}, {:0.4g}'
              .format(time.time()-start, t,
              solver.dt,0,0 #solver.get_rhsevals(),
              #np.abs(solver.get_error()).max()
              ))
        depth.append([r.depth for r in reaches])
        
    time_elapsed = time.time() - start 
    
    for state_id, count in sorted(c.items()):
        state = reaches[state_id // (len(p.solutes) + 1)]
        solute_id = state_id % (len(p.solutes) + 1)
        if solute_id:
            solute = p.solutes[solute_id - 1]
            state_name = str(state) + '.' + str(solute)
            # state_val = state.Solute(solute).state
        else:
            state_name = str(state)
            # state_val = state.
        print('{:>20}: {}x greatest error'.format(state_name, count))
        
    return time_elapsed, depth, solver, c

if __name__ == '__main__':
    
    parser = argparse.ArgumentParser()
    parser.add_argument('segment_length', type=int, help='Length of river segments in m')
    parser.add_argument('-X','--tracer', action='store_true', help='Use -x to create a tracer')
    parser.add_argument('-t','--threads', action='store', default=1, type=int, help='Number of threads')
    args = parser.parse_args()
    
    def run():
        return test(args.segment_length, args.tracer, args.threads)
    
    t = timeit.Timer(run)
    
    print('l={args.segment_length:4} m, X={args.tracer}, t={args.threads}: {t:0.5g}s'.format(args=args, t=t.timeit(1)))
    