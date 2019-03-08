import cmf
import unittest
import pickle
import datetime


class TimeTest(unittest.TestCase):

    def setUp(self):
        self.t1 = cmf.Time(1, 1, 2018, 12, 13, 59)
        self.t2 = cmf.Time(2, 1, 2018, 12, 13, 59)

    def test_time_pickle(self):
        t = cmf.Time(1, 1, 2018)
        pt = pickle.dumps(t)
        t2 = pickle.loads(pt)
        self.assertEqual(t, t2)

    def test_time_greater(self):
        self.assertTrue(self.t2 > self.t1)

    def test_time_smaller(self):
        self.assertTrue(self.t1 < self.t2)

    def test_time_diff(self):
        self.assertEqual(self.t2 - self.t1, cmf.day)

    def test_time_mult(self):
        self.assertEqual(cmf.sec * 86400, cmf.day)

    def test_time_div(self):
        self.assertEqual(cmf.day / cmf.sec, 86400.0)
        self.assertEqual(cmf.day / 86400.0, cmf.sec)

    def test_time_add(self):
        self.assertEqual(self.t1 + cmf.day, self.t2)

    def test_time_as_datetime(self):
        self.assertEqual(self.t2.as_datetime(),
                         datetime.datetime(2018, 1, 2, 12, 13, 59))

    def test_time_as_timedelta(self):
        t0 = datetime.datetime(1899, 12, 30, 0, 0, 0, 0)
        td = datetime.datetime(2018, 1, 2, 12, 13, 59) - t0
        self.assertEqual(self.t2.as_timedelta(), td)

    def test_time_from_datetime(self):
        self.assertEqual(cmf.AsCMFtime(datetime.datetime(2018, 1, 2, 12, 13, 59)),
                         self.t2)


class TimeseriesTest(unittest.TestCase):

    def test_timeseries_basic(self):
        ts = cmf.timeseries(cmf.Time(), cmf.h)
        self.assertEqual(len(ts), 0)
        ts.add(1.0)
        self.assertEqual(len(ts), 1)
        self.assertEqual(ts.end, cmf.h)

    def test_timeseries_from_scalar(self):
        ts = cmf.timeseries.from_scalar(1.0)
        self.assertEqual(len(ts), 1)
        self.assertEqual(ts[0], 1.0)
        with self.assertRaises(IndexError):
            _ = ts[1]
        self.assertEqual(ts[cmf.Time(1, 1, 2018)], 1.0)

    def test_timeseries_pickle(self):
        begin = cmf.Time(1, 1, 2012)
        step = cmf.h
        ots = cmf.timeseries.from_sequence(begin, step, range(100))
        pickled_ts = pickle.dumps(ots)
        nts = pickle.loads(pickled_ts)
        self.assertEqual(nts.begin, ots.begin)
        self.assertEqual(nts.step, ots.step)
        self.assertEqual(len(nts), len(ots))

        for o, n in zip(ots, nts):
            self.assertEqual(o, n)

        for t in nts.iter_time():
            self.assertEqual(ots[t], nts[t])


