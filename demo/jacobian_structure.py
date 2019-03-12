"""
CMF-Demo to explore the sparse structure of the Jacobian

Use case: A soil column with a ditch at its side

  ##_|
"""
import cmf
import numpy as np
from matplotlib.animation import FuncAnimation
import matplotlib.pylab as plt
from matplotlib.lines import Line2D


def connector_matrix(project: cmf.project):
    """
    Returns a matrix that shows the connectivity between the given states

    :param states: A sequence of states to create the matrix
    :param size: Large matrices can compressed with a factor.
    :return: A symmetric 2d matrix with 1 for connected states and 0 for
            unconnected states. Compressed matrices contain larger numbers for
            the count of connection in the compressed field
    """
    posdict = {}
    storages = project.get_storages()
    l_sol = len(project.solutes) + 1
    l = len(storages) * l_sol
    size = (l, l)
    jac = np.zeros(size, dtype=int)
    # Get the position of each node_id
    for i, a in enumerate(storages):
        posdict[a.node_id] = i
        # print(f'#{i:4}: {str(a):24}: node_id={a.node_id}')
    indptr = []
    indices = []
    data = []
    for i, a in enumerate(storages):
        indptr.append(len(data))
        for f, t in list(a.fluxes(cmf.Time())) + [(0, a)]:
            j = posdict.get(t.node_id, -1)
            if j >= 0:
                jac[j * l_sol:(j + 1) * l_sol, i * l_sol] = 1
                data.extend([1] * l_sol)
                indices.extend(range(j * l_sol, (j + 1) * l_sol))
    return jac, data, indices, indptr


class RichardsNextToDitch:
    def __init__(self):
        p = cmf.project('X')
        c: cmf.Cell = p.NewCell(0, 0, 0, 1, True)
        ditch: cmf.OpenWaterStorage = p.NewOpenStorage('ditch', 1, 0, -1, 0.5)
        gw: cmf.DirichletBoundary = p.NewOutlet('gw', 0, 0, -1)

        # Add 10 layers
        for d in np.arange(0.05, 1, 0.05):
            l = c.add_layer(d, cmf.VanGenuchtenMualem(1, 0.5, 0.01, 1.5))
            cmf.Darcy(l, ditch, 1, 1)
        c.install_connection(cmf.Richards)

        cmf.Richards(c.layers[-1], gw)

        c.saturated_depth = 1.0
        ditch.depth = 1.0

        self.p = p
        self.c = c
        self.ditch = ditch
        self.gw = gw

    def __len__(self):
        return self.p.get_states().size()


    def run(self):
        solver = cmf.CVodeDense(self.p, 1e-9)
        p, c, ditch, gw = self.p, self.c, self.ditch, self.gw
        # Initial Conditions
        c.saturated_depth = 1.0
        ditch.depth = 1.0
        result = []
        jacobians = []

        for t in solver.run(cmf.Time(1, 1, 2018), cmf.Time(2, 1, 2018), cmf.min):
            SD = 1 - c.saturated_depth
            DD = ditch.depth
            GW = gw(t) * 1000
            print(f't={t:%H:%M} SH={SD:6.3f} m, DD={DD:6.3f} m, GW(t)={GW:0.5g} l/day')
            result.append([SD, DD, GW])
            jacobians.append(solver.get_jacobian())
        return np.array(result), np.array(jacobians)




dt = 5
days = 2

class Animator(FuncAnimation):

    def __init__(self, model):
        self.model = model
        gs = plt.GridSpec(2, 2)
        self.figure = plt.figure()
        self.ax = [
            plt.subplot(gs[0:,0]),
            plt.subplot(gs[0, 1]),
            plt.subplot(gs[1, 1]),
        ]
        self.sd_line, = self.ax[1].plot([0, days*24*60/dt], [-1, 1], 'g-', label='SD[m]')
        self.dd_line, = self.ax[1].plot([0, 24*60/dt], [0, 1], 'b-', label='DD[m]')
        self.sp_line, = self.ax[0].plot([-1.5, 0], [-1, 0], 'r-', label='$\theta$[m]')
        dummy_jac = np.random.uniform(-5, 5, size=[len(self.model)] * 2)
        self.jac_img = self.ax[2].imshow(dummy_jac, aspect='equal')
        self.solver = cmf.CVodeDense(self.model.p, 1e-9)
        self.title = plt.suptitle('start...')
        super().__init__(self.figure, self.draw, self.run(), interval=10, repeat=False)

    def run(self):

        self.solver.reset()
        # Initial Conditions
        self.model.c.saturated_depth = 1.0
        self.model.ditch.depth = 1.0
        self.model.c.surfacewater.depth = 0.05
        if self.model.p.solutes:
            X, = self.model.p.solutes
            for l in self.model.c.layers:
                l.conc(X, 0.0)
            self.model.ditch.conc(X, 1.0)
            self.model.c.surfacewater.conc(X, 1.0)

        self.sp_line.set_data([], [])
        self.sd_line.set_data([], [])
        self.dd_line.set_data([], [])
        self.solver.t = cmf.Time(1, 1, 2018)
        for t in self.solver.run(self.solver.t, self.solver.t + cmf.day * days, cmf.min * dt):
            yield t

    @staticmethod
    def add_data_to_line(line: Line2D, value:float):
        x, y = line.get_data()
        x = list(x) + [len(x)]
        y = list(y) + [value]
        line.set_data(x, y)

    def draw(self, frame):
        t = frame
        p, c, ditch, gw = self.model.p, self.model.c, self.model.ditch, self.model.gw
        SP = c.layers.matrix_potential.copy()
        SD = 1 - c.saturated_depth
        DD = ditch.depth
        GW = gw(t) * 1000

        self.title.set_text(str(t))
        self.add_data_to_line(self.sd_line, SD)
        self.add_data_to_line(self.dd_line, DD)
        self.sp_line.set_data(SP, -c.layers.lower_boundary)
        jac = self.solver.get_jacobian()
        jmax = jac.max()
        jmin = jac.min()
        jac[jac == 0] = np.nan
        self.jac_img.set_array(jac)
        print(f't={t:%H:%M} SH={SD:6.3f} m, DD={DD:6.3f} m, GW(t)={GW:0.5g} l/day, j={jmin:0.5g} - {jmax:0.5g}')
        return [self.title, self.sd_line, self.dd_line, self.sp_line, self.jac_img]


if __name__ == '__main__':
    model = RichardsNextToDitch()

    #anim = Animator(model)
    jac, d, i, ip = connector_matrix(model.p)
    plt.imshow(jac, aspect='equal')
    plt.show()


