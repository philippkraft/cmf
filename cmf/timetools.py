

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   cmf is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with cmf.  If not, see <http://www.gnu.org/licenses/>.
#   
import time
from .cmf_core import Time, day


def datetime_to_cmf(date):
    """Converts a python datetime to cmf.Time"""
    return Time(date.day, date.month, date.year, date.hour, date.minute, date.second, date.microsecond / 1000)


class timerange:
    """Creates a generator of cmf.Time, similar to the Python range function"""
    def __init__(self, start, stop, step=day):
        self.start = start
        self.stop = stop
        self.step = step

    def __iter__(self):
        for i in range(len(self)):
            yield self.start + self.step * i

    def __repr__(self):
        return 'cmf.timerange({}, {}, {})'.format(self.start, self.stop, self.step)

    def __len__(self):
        return int((self.stop - self.start) / self.step)

    def __getitem__(self, item):
        n = len(self)
        if type(item) is int:
            if item < 0:
                item += n
            if item >= n:
                raise IndexError('Item [{}] not in {}'.format(item, self))
            return self.start + self.step * item

        elif type(item) is slice:
            i_start, i_stop, i_step = item.indices(n)
            try:
                return timerange(self[int(i_start)], self[int(i_stop)], self.step * int(i_step))
            except TypeError:
                raise TypeError('timerange[item]: Item must be either int, slice of ints or a sequence of ints')

        else:
            try:
                return [
                    self[int(single_item)]
                    for single_item in item
                ]
            except TypeError:
                raise TypeError('timerange[item]: Item must be either int, slice of ints or a sequence of ints')


class StopWatch:
    """A stopwatch to estimated the total time of a process  
    
    Creating a StopWatch:
    >>>stopwatch=StopWatch(start, stop)
    Start and end are indicators to describe the progress of a process. 
    Start is the indicator value at the beginning of the process. As default they are 0.0 and 1.0.
    
    Starting the StopWatch again:
    >>>stopwatch.restart()
    
    Getting the elapsed time, the total time and the remaining time of the process in seconds:
    >>>elapsed, total, remaining = stopwatch(progress)
    Where progress is a process progress indicator matching start and stop 
    
    Example:
    stopwatch=StopWatch(0,10)
    for i in range(10):
        time.sleep(1)
        print('elapsed = %0.2fs, total= %0.2fs, remaining = %0.2fs' % stopwatch(i+1))
    
    
    """
    def __init__(self, start=0.0, stop=1.0):
        self.start = start
        self.stop = stop
        self.t0 = time.time()

    def restart(self):
        """Restarts the stopwatch"""
        self.t0 = time.time()

    def __call__(self, progress):
        """
        Returns the progress in wall clock time
        :param progress: The current position in the stopwatch
        :return: A 3-tuple of elapsed, total and remaining seconds of the task
        """
        now = time.time()
        elapsed = (now-self.t0)
        if progress > self.start:
            total = elapsed * (self.stop-self.start)/(progress-self.start)
        else:
            total = 0.0
        return elapsed, total, total - elapsed


