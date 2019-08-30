import cmf
p=cmf.project('A B C')
wstor = p.NewStorage('w')
A,B,C = p.solutes
sA=wstor[A]
print('wstor[A].reactions')
print('\n'.join('    .' + d for d in dir(sA.reactions)))
r = cmf.SoluteConstantFluxReaction(1)
sA.reactions = [r]

print(sA.reactions[0])
