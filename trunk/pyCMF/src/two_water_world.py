import cmf
import visualCMF

p=cmf.project()
c=p.NewCell(0,0,0,25)
r_micro=cmf.BrooksCoreyRetentionCurve()
r_micro.SetPorosity(0.4,0)
r_curve=cmf.RCurve(r_micro,1)

