import cmf
from numpy import zeros,float
class Jacobian(object):
    """Approximates the jacobian for a cmf solver
    J[i,j] = (dxdt(S_i,t)[j]-dxdt(S,t)[j])/delta
    S is the state vector
    S_i equals S, except for S_i[i]=S[i]+delta
    delta is the finite difference to approximate the Jacobian.
        delta should be a small number, but big enough to avoid floating point errors. 
        1e-6 to 1e-9 should be nice values
    
    Usage to show the jacobian:
        # Allocate memory for the jacobian
        jac = Jacobian(solver,delta)
        # Calculate the Jacobian
        J = jac()
        # Show the Jacobian
        imshow(jac(),interpolation='nearest')
    """
    def __init__(self,solver,delta=1e-6):
        """solver is a cmf integrator, delta is the amount the state should be changed
        """
        self.jacobian = zeros((solver.size(),solver.size()),dtype=float)
        self.solver=solver
        self.delta = delta
    @property
    def dxdt(self):
        """Returns the current right hand side of the ODE for the current states and the current time
        """
        return self.solver.get_dxdt(solver.t)
    def __len__(self):
        return self.solver.size()
    @property
    def t(self):
        return self.solver.t
    def __call__(self):
        dxdt = self.dxdt
        jac = self.jacobian
        for i in range(len(solver)):
            solver.set_state(i,solver.get_state(i)+self.delta)
            jac[i] = (solver.get_dxdt(solver.t) - dxdt)/self.delta
            solver.set_state(i, solver.get_state(i)-self.delta)
        return jac
        
