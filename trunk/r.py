import cmf
print cmf.VERSION
p=cmf.project('X')
X,=p.solutes
c=p.NewCell(0,0,0,1000)
c.add_layer(0.1, cmf.VanGenuchtenMualem())
c.surfacewater_as_storage()
c.rain.flux[0] = 1.0
c.rain.concentration[X] = 1.0
con = c.surfacewater.get_connection(c.layers[0])
t0=cmf.Time()
print con.conc(t0,X)