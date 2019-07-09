import cmf

class SchmittTriggerSourceConnection:
    """
    A Schmitt-trigger for hysterese regulation effects, triggered by
    the water source.

    This connection emulates the behaviour of manually
    switched pumps or the regulation of a dam / reservoir.
    It is similar to the SchmittTriggerTargetConnection,
    but that is triggered by the state of the target.

    If the volume of the water source is greater then the upper regulation level,
    the flux is switched on, if the level is smaller then the lower
    regulation level, it is switched off. Between those levels, the
    flux stays in the same state as before.

    A trigger where the flow is turned on if the source has more than
    2 m³ water stored, and turned off if it has less then 1m³ with 10m³/day
    flow rate is created as:

    >>> trigger = SchmittTriggerSourceConnection(src, tgt, 1, 2, 10)

    This connection needs to be checked if the flux needs to be turned on
    or off, the trigger needs to be called like a function at every timestep:

    >>> for t in solver.run(...):
    >>>     trigger()
    """

    def __init__(self, source, target,
                 lower_regulation_level, upper_regulation_level, flow):
        """
        Creates a source regulated Schmitt trigger
        :param source: The source of the water (must be a water storage)
        :param target: The target of the water (can be any node)
        :param lower_regulation_level: The lower regulation level in m³
        :param upper_regulation_level: The upper regulation level in m³
        :param flow: The flow, when it is turned on in m³/day
        """
        self.nodes = source, target
        self.range = lower_regulation_level, upper_regulation_level

        self.flow = flow
        self.cmf_connection = cmf.ConstantFlux(
            source, target, flow
        )

    def is_running(self):
        """
        Returns True if the trigger is currently turned on
        """
        return self.cmf_connection.MaxFlux > 0

    def current_flow(self, t):
        """
        Returns the current flow in m³/day from source to target
        (0 if turned off or the source is empty)
        :param t: the actual time
        """
        return self.source.flux_to(self.target, t)

    def can_run(self):
        """
        Returns True, if the flow can be turned on
        """
        return (
                self.volume > self.range[1] or
                (self.is_running() and self.volume > self.range[0])
        )

    @property
    def source(self):
        return self.nodes[0]

    @property
    def target(self):
        return self.nodes[1]

    @property
    def volume(self):
        return self.source.volume

    def __call__(self, solver: cmf.Integrator=None):
        """
        Checks the current state and regulates the flow from source to target
        :param solver: A cmf.Integrator (optional). If given, the integrator is reset
                       to deal with the abrupt flow change
        """
        if self.can_run() and not self.is_running():
            self.cmf_connection.MaxFlux = self.flow
            if solver:
                solver.reset()
        elif self.is_running() and not self.can_run():
            self.cmf_connection.MaxFlux = 0.0
            if solver:
                solver.reset()


class SchmittTriggerTargetConnection:
    """
    A Schmitt-trigger for hysterese regulation effects, triggered by
    the water target state.

    This connection emulates the behaviour of manually
    switched pumps or the regulation of a dam / reservoir.
    It is similar to the SchmittTriggerSourceConnection,
    but that is triggered by the state of the source.

    If the volume of the water target is greater then the upper regulation level,
    the flux is switched off, if the level is smaller then the lower
    regulation level, it is switched on. Between those levels, the
    flux stays in the same state as before.

    A trigger where the flow is turned off if the target has more than
    2 m³ water stored, and turned on if it has less then 1m³ with 10m³/day
    flow rate is created as:

    >>> trigger = SchmittTriggerTargetConnection(src, tgt, 1, 2, 10)

    This connection needs to be checked if the flux needs to be turned on
    or off, the trigger needs to be called like a function at every timestep:

    >>> for t in solver.run(...):
    >>>     trigger()
    """

    def __init__(self, source, target,
                 lower_regulation_level, upper_regulation_level, flow):
        """
        Creates a source regulated Schmitt trigger
        :param source: The source of the water (must be a water storage)
        :param target: The target of the water (can be any node)
        :param lower_regulation_level: The lower regulation level in m³
        :param upper_regulation_level: The upper regulation level in m³
        :param flow: The flow, when it is turned on in m³/day
        """
        self.nodes = source, target
        self.range = lower_regulation_level, upper_regulation_level

        self.flow = flow
        self.cmf_connection = cmf.ConstantFlux(
            source, target, flow
        )

    def is_running(self):
        """
        Returns True if the trigger is currently turned on
        """
        return self.cmf_connection.MaxFlux > 0

    def current_flow(self, t):
        """
        Returns the current flow in m³/day from source to target
        (0 if turned off or the source is empty)
        :param t: the actual time
        """
        return self.source.flux_to(self.target, t)

    def can_run(self):
        """
        Returns True, if the flow can be turned on
        """
        return (
                self.volume < self.range[0] or
                (self.is_running() and self.volume < self.range[1])
        )
    @property
    def source(self):
        return self.nodes[0]

    @property
    def target(self):
        return self.nodes[1]
    @property
    def volume(self):
        return self.target.volume

    def __call__(self, solver=None):
        """
        Checks the current state and regulates the flow from source to target
        :param solver: A cmf.Integrator (optional). If given, the integrator is reset
                       to deal with the abrupt flow change
        """
        if self.can_run() and not self.is_running():
            self.cmf_connection.MaxFlux = self.flow
            if solver:
                solver.reset()
        elif self.is_running() and not self.can_run():
            self.cmf_connection.MaxFlux = 0.0
            if solver:
                solver.reset()


class SchmittTriggerTestSetup:
    """
    A simple setup to test the Schmitt-trigger connections
    nodes:
     - n: Neumann boundary with constant flux to s1
     - s1: Storage 1, refilled by n, emptied to s2 with the Schmitt-Trigger
     - s2: Storage 2, refilled from s1, emptied to outlet o with a linear storage connection
     - o: Outlet
    """

    def __init__(self, factor, speed=24, duration=6*24):
        """
        :param factor: How much faster is the pump than the recharge from outside
        :param speed: m³/day recharge of S1
        :param duration: Duration of the simulation in hours
        """
        self.duration = duration
        self.project = p = cmf.project()
        self.s1 = s1 = p.NewStorage('s1')
        self.s2 = s2 = p.NewStorage('s2')
        self.out = out = p.NewOutlet('o')
        cmf.LinearStorageConnection(s2, out, 2 / speed, residual=0.5)
        self.ns1 = ns1 = p.NewNeumannBoundary('ns1', s1)
        ns1.flux = speed
        self.trigger = SchmittTriggerSourceConnection(
            s1, s2, 1, 2, factor * speed
        )

    def run(self):
        vol = []
        flow = []
        solver = cmf.CVodeAdams(self.project)
        for t in solver.run(cmf.h * 0, cmf.h * self.duration, cmf.min * 15):
            self.trigger(solver)
            print(f'{t/cmf.h:5.1f}h: V1={self.s1.volume:4.2f}, '
                  f'{self.trigger.can_run()}/{self.trigger.is_running()}', end='\n')
            vol.append([self.s1.volume, self.s2.volume])
            flow.append(
                [self.ns1.flux_to(self.s1, t),
                 self.s1.flux_to(self.s2, t),
                 self.s2.flux_to(self.out, t)
                 ]
            )
        print()
        return vol, flow

if __name__ == '__main__':

    sts = SchmittTriggerTestSetup(6, 4, 10 * 24)
    vol, flux = sts.run()

    import matplotlib.pyplot as plt
    fig, ax = plt.subplots(2, sharex=True)
    ax[0].plot(vol)
    ax[0].set_ylabel('Stored volume in m³')
    ax[0].legend('S1 S2'.split(), loc=0)
    ax[1].plot(flux)
    ax[1].set_ylabel('Fluxes')
    ax[1].legend('N->S1 S1->S2 S2->out'.split(), loc=0)