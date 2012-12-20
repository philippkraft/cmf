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
        return self.solver.get_dxdt(self.t)
    def __len__(self):
        return self.solver.size()
    @property
    def t(self):
        return self.solver.t
    def __call__(self):
        dxdt = self.dxdt
        jac = self.jacobian
        for i in range(len(self)):
            # Change state i to S[i]+delta
            self.solver.set_state(i,self.solver.get_state(i)+self.delta)
            # Get approx. derivate df/dS = (f(S_i)-f(S))/delta
            jac[i] = (self.solver.get_dxdt(self.t) - dxdt)/self.delta
            # Reset change of S[i]
            self.solver.set_state(i, self.solver.get_state(i)-self.delta)
        return jac
        
