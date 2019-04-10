import cmf
from datetime import datetime, timedelta
from pylab import plot, twinx, ylabel, xlabel, show

# Create project with 1 cell and 1 water storage of 1000mm capacity
p = cmf.project()
cell = p.NewCell(0, 0, 0, 1000)
layer = cell.add_layer(1.0)

# Set summertime, when the living is easy... (1)
summer = cmf.Weather(Tmin=19, Tmax=28, rH=50, wind=4.0,
                     sunshine=0.9, daylength=14, Rs=26)
cell.set_weather(summer)

# Initial condition (4)
layer.volume = 400.0

# ET-Method (3)
et_pot_turc = cmf.TurcET(layer, cell.transpiration)

# Stress conditions (5, 6)
stress = cmf.VolumeStress(300, 100)
cell.set_uptakestress(stress)

# A solver
solver = cmf.HeunIntegrator(p)
solver.t = datetime(2018, 5, 1)
et_act = cmf.timeseries(solver.t, cmf.day)
volume = cmf.timeseries(solver.t, cmf.day)
while solver.t < datetime(2018, 10, 1):
    et_act.add(cell.transpiration(solver.t))
    volume.add(layer.volume)
    solver(solver.t + cmf.day)

# And a plot
plot(et_act, c='g')
ylabel(r'$ET_{act} \left[\frac{mm}{day}\right]$')
twinx()
plot(volume, c='b')
ylabel('Volume in mm')
show()
