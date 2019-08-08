

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
import struct
from .cmf_core import Time, day, timeseries, sec, ms

try:
    import pandas as pd
except ImportError:
    pd = None


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


# Extend timeseries

def timeseries_iter_time(self):
    """
    Returns an iterator to iterate over each timestep
    """
    for i in range(len(self)):
        yield self.begin + self.step * i


timeseries.iter_time = timeseries_iter_time


def timeseries_to_buffer(self):
    """Returns a binary buffer filled with the data of self"""
    return struct.pack('qqqq{}d'.format(self.size()), self.size(), self.begin.AsMilliseconds(),
                       self.step.AsMilliseconds(), self.interpolationpower(), *self)


timeseries.to_buffer = timeseries_to_buffer


def timeseries_to_file(self, f):
    """ Saves a timeseries in a special binary format.
    The format consists of 4 integers with 64 bit, indicating the milliseconds after the 31.12.1899 00:00 of the beginning of the timeseries, the milliseconds of the time step,
    the interpolation power and the number of values. The following 64 bit floats, are the values of the timeseries
    """
    if isinstance(f, str):
        f = open(f, 'wb')
    elif not hasattr(f, 'write'):
        raise TypeError(
            "The file f must be either an object providing a write method, like a file, or a valid file name")
    f.write(self.to_buffer())


def timeseries___getstate__(self):
    return dict(size=len(self),
                begin=self.begin.AsMilliseconds(),
                step=self.step.AsMilliseconds(),
                interpolationpower=self.interpolationpower(),
                values=self.as_array()
                )


timeseries.__getstate__ = timeseries___getstate__


def timeseries___setstate__(self, data):
    begin = ms * data['begin']
    step = ms * data['step']
    self.__init__(begin, step, data['interpolationpower'])
    self.extend(data['values'])


timeseries.__setstate__ = timeseries___setstate__


def timeseries_from_sequence(cls, begin, step, sequence, interpolation_mode=1):
    res = cls(begin, step, interpolation_mode)
    res.extend(sequence)
    return res


timeseries.from_sequence = classmethod(timeseries_from_sequence)


def timeseries_from_buffer(cls, buf):
    import numpy as np
    header_length = struct.calcsize('qqqq')
    header = struct.unpack('qqqq', buf[:header_length])
    res = cls(header[1] * ms, header[2] * ms, header[3])
    res.extend(np.fromstring(buf[header_length:], dtype=float))
    return res


timeseries.from_buffer = classmethod(timeseries_from_buffer)


def timeseries_from_file(cls, f):
    """ Loads a timeseries saved with to_file from a file
    Description of the file layout:
    byte:
    0   Number of (int64)
    8   Begin of timeseries (in ms since 31.12.1899 00:00) (int64)
    16  Step size of timeseries (in ms) (int64)
    24  Interpolation power (int64)
    32  First value of timeseries (float64)
    """
    if isinstance(f, str):
        f = open(f, 'rb')
    elif not hasattr(f, 'read'):
        raise TypeError(
            "The file f must either implement a 'read' method, like a file, or must be a vild file name")
    header_length = struct.calcsize('qqqq')
    header = struct.unpack('qqqq', f.read(header_length))
    res = cls(header[1] * ms, header[2] * ms, header[3])
    res.extend(struct.unpack('%id' % header[0], f.read(-1)))
    return res


timeseries.from_file = classmethod(timeseries_from_file)


# Extend timeseries with pandas tools if pandas is present
if pd:
    def pandas_series_to_timeseries(cls, series, regularize='warning'):

        import numpy as np

        if (series.index[1:] - series.index[:-1] == series.index[1] - series.index[0]).all():
            s = series
        else:
            if regularize:
                s = series.resample(series.index[1] - series.index[0]).interpolate('time')
                if regularize in ['w', 'warn', 'warning']:
                    from logging import warning
                    warning(('While converting pandas series "{n}" to cmf.timeseries, '
                            'data has been regularized and consists now of {o} original values '
                            'and {i} interpolated values').format(
                        n=series.name, o=series.count(), i=s.count() - series.count())
                    )
            else:
                raise ValueError('The pandas timeseries is not regular. '
                                 'Try cmf.timeseries.from_series(series, regularize=True)'
                                 'to allow for interpolation')

        tstep = sec * (s.index[1] - s.index[0]).total_seconds()
        tstart = datetime_to_cmf(s.index[0].to_pydatetime())
        array = np.array(s)
        return cls.from_array(tstart, tstep, array)


    def timeseries_to_pandas(self):
        """
        Returns the timeseries as a pandas Series object
        :return: A pandas.Series object with the timesteps as index
        """
        import pandas as pd
        import numpy as np

        index = pd.date_range(self.begin.as_datetime(), (self.end - self.step).as_datetime(), len(self))

        return pd.Series(data=self.as_array(), index=index)


    timeseries.from_pandas = classmethod(pandas_series_to_timeseries)
    timeseries.to_pandas = timeseries_to_pandas


