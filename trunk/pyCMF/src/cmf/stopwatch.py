import time
class StopWatch :
    """A stopwatch to estimated the total time of a process  
    
    Creating a StopWatch:
    >>>stopwatch=StopWatch(start,stop) 
    Start and end are indicators to describe the progress of a process. 
    Start is the indicator value at the beginning of the process. As default they are 0.0 and 1.0.
    
    Starting the StopWatch again:
    >>>stopwatch.start()
    
    Getting the elapsed time, the total time and the remaining time of the process in seconds:
    >>>elapsed,total,remaining = stopwatch(progress)
    Where progress is a process progress indicator matching start and stop 
    
    Example:
    stopwatch=StopWatch(0,10)
    for i in range(10):
        time.sleep(1)
        print 'elapsed = %0.2fs, total= %0.2fs, remaining = %0.2fs' % stopwatch(i+1)
    
    
    """
    def __init__(self,start=0.0,stop=1.0):
        self.start=start
        self.stop=stop
        self.t0=time.clock()
    def start(self):
        self.t0=time.clock()
    def __call__(self,progress):
        now=time.clock()
        elapsed=(now-self.t0)
        if progress>self.start :
            total= elapsed * (self.stop-self.start)/(progress-self.start)
        else:
            total= 0.0
        return elapsed, total, total - elapsed
if __name__=='__main__':
    sw=StopWatch(0,1000)
    for i in range(1000):
        #time.sleep(0.01)
        print 'elapsed = %0.5fs, total= %0.5fs, remaining = %0.5fs' % sw(i+1)
    
