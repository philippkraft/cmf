import cmf
from matplotlib import pyplot as plt

p = cmf.project('X')
X, = p.solutes

V1 = p.NewStorage('V1', 0, 0, 0)
V2 = p.NewStorage('V2', 1, 0, 0)
V3 = p.NewStorage('V3', 0, 0, 1)
V4 = p.NewStorage('V4', 1, 0, 1)

NB = p.NewNeumannBoundary('NB', V1)
NB.flux = 1
NB.concentration[X] = 1

DB = p.NewOutlet('DB', -1, 0, 0)

q_nb_1 = NB.connection_to(V1)
q_1_2 = cmf.LinearStorageConnection(V1, V2, 0.1)
q_1_3 = cmf.LinearStorageConnection(V1, V3, 0.5)
q_2_4 = cmf.LinearStorageConnection(V2, V4, 1)
q_4_3 = cmf.LinearStorageConnection(V4, V3, 0.5)
q_3_out = cmf.LinearStorageConnection(V3, DB, 1)

V2[X].decay = 0.5
V4[X].decay = 0.5
V3[X].decay = 0.1

for S in [V1, V2, V3, V4]:
    S.volume = 0.0

solver = cmf.CVodeDiag(p)
solver.t = cmf.Time()
solver.integrate_until(cmf.year)
print(solver.get_info())
t = solver.t
plt.imshow(solver.get_jacobian(), vmin=-1, vmax=1, cmap=plt.cm.coolwarm)
plt.colorbar()