"""
Calculates a matrix from the connected states. Useful to explore numerical problems
"""
import numpy
import cmf

def connector_matrix(states, compression_factor=1):
    """
    Returns a matrix that shows the connectivity between the given states

    :param states: A sequence of states to create the matrix
    :param size: Large matrices can compressed with a factor.
    :return: A symmetric 2d matrix with 1 for connected states and 0 for
            unconnected states. Compressed matrices contain larger numbers for
            the count of connection in the compressed field
    """
    l = len(states)
    size = (l // compression_factor, l // compression_factor)
    jac = numpy.zeros(size, dtype=int)
    posdict = {a.node_id: i for i, a in enumerate(states)}
    for i, a in enumerate(states):
        for f, t in a.fluxes(cmf.Time()):
            j = posdict.get(t.node_id)
            if j:
                jac[i * size[0] // l, j * size[1] // l] += 1
    return jac
