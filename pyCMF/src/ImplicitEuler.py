import cmf
from numpy import array
class Euler:
    def __init__(self,state_variables):
        self.states=state_variables
        self.old_states=[s.get_state() for s in self.states]
        self.itercount=0
        self.error=1000
    def recover(self):
        for i,s in enumerate(self.states):
            s.set_state(self.old_states[i])
    def iterate(self,t,dt):
        self.itercount+=1
        derivates=array([s.Derivate(t) for s in self.states])
        compare_states=array([s.state for s in self.states])
        for i,s in enumerate(self.states):
            s.set_state(self.old_states[i]+derivates[i]*dt.AsDays())
        new_states=array([s.state for s in self.states])
        self.error=max(abs(compare_states-new_states))        
        return list(new_states)+[self.error]
                
        
                     
        