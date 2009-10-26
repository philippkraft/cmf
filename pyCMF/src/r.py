import cmf
p=cmf.project()
dem=cmf.Raster(shape=(10,10),cellsize=(10,10))
for c,r in dem:
    dem[c,r]=(((c-50)*0.1)**2 - ((r-50)*0.1)**2)
cells=cmf.cells_from_dem(p, dem)
bc=cmf.BrooksCoreyRetentionCurve()
prof=cmf.profile()
for d in [0.1,0.3,0.5,0.75,1.0,1.3,1.6,2.0,2.5,3.0,4.0]:
    prof.append(d, bc)
print "layers added:",    cmf.add_layers_to_cells(p,lambda x,y,z: prof, lambda x,y,z : (z+1)*0.5)

cmf.connect_cells_with_flux(p,cmf.Richards_lateral)


