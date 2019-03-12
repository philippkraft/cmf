import cmf
import numpy as np
from itertools import chain


def connector_matrix(states, compression_factor=1):
    """
    Returns a matrix that shows the connectivity between the given states

    :param states: A sequence of states to create the matrix
    :param compression_factor: Large matrices can compressed with a factor.
    :return: A symmetric 2d matrix with 1 for connected states and 0 for
            unconnected states. Compressed matrices contain larger numbers for
            the count of connection in the compressed field
    """
    posdict = {}
    le = len(states)
    size = (le // compression_factor, le // compression_factor)
    jac = np.zeros(size, dtype=int)
    for i, a in enumerate(states):
        posdict[a.node_id] = i
        jac[i * size[0] // l, i * size[1] // l] += 1
    for i, a in enumerate(states):
        for f, t in a.fluxes(cmf.Time()):
            j = posdict.get(t.node_id)
            if j:
                jac[i * size[0] // le, j * size[1] // le] += 1
    return jac


def connected_states(states):
    """
    Get a set of all connected nodes, indicated by the node_id's

    :param states: An iterable of states
    :return: a set of node_id tuples
    """
    def get_connection_tuples(state):
        return {
            tuple(sorted((state.node_id, n.node_id)))
            for n in state.connected_nodes
        }
    return set(chain.from_iterable(get_connection_tuples(s) for s in states))


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

    def __init__(self, solver, delta=1e-6):
        """solver is a cmf integrator, delta is the amount the state should be changed
        """
        self.jacobian = np.zeros((solver.size(), solver.size()), dtype=float)
        self.solver = solver
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
            self.solver.set_state(i, self.solver.get_state(i) + self.delta)
            # Get approx. derivate df/dS = (f(S_i)-f(S))/delta
            jac[i] = (self.solver.get_dxdt(self.t) - dxdt) / self.delta
            # Reset change of S[i]
            self.solver.set_state(i, self.solver.get_state(i) - self.delta)
        return jac
