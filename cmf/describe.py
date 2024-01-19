# -*- coding: utf-8 -*-


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

"""
Author: Florian U. Jehn & Philipp Kraft
Original version from ACME (https://github.com/zutn/ACME)

The version here is different - the describe function tries to describe everything
"""

from __future__ import print_function, division, absolute_import, unicode_literals
import io
from . import cmf_core as cmf


def describe(cmfobject, out=None):
    """
    Describes a cmf object in a file like object or returns the description.

    If no special description method for the class of cmfobject is available,
    the function writes the string representation

    :param cmfobject: any cmf object eg. project, Cell, timeseries
    :param out: filelike object, if None function returns the description as string
    :return: If out is None it returns the description as string, else no return
    """
    if out is None:
        out = io.StringIO()
        return_str = True
    else:
        return_str = False

    def write(indent_level=0, text=''):
        """
        Helper function to write output to the out stream with identation
        """
        out.write('    ' * indent_level + text + '\n')

    def iterable(obj):
        try:
            _ = iter(obj)
            return True
        except (TypeError, AttributeError):
            return False

    if isinstance(cmfobject, cmf.project):
        _describe_project(cmfobject, write)
    elif isinstance(cmfobject, cmf.Cell):
        _describe_cell(cmfobject, write)
    elif isinstance(cmfobject, cmf.MeteoStation):
        _describe_meteo(cmfobject, write)
    elif isinstance(cmfobject, cmf.RainfallStation):
        _describe_rain(cmfobject, write)
    elif isinstance(cmfobject, cmf.flux_node):
        _describe_node(0, cmfobject, write)
    elif isinstance(cmfobject, cmf.timeseries):
        write(0, _describe_timeseries(cmfobject))

    elif cmfobject == str(cmfobject):
        # Is string
        return cmfobject
    elif iterable(cmfobject):
        # Is other iterable
        write(0, str(cmfobject) + ':')
        for o in cmfobject:
            write(1, ' - ' + describe(o))
    else:
        # Return as string
        write(0, str(cmfobject))

    if return_str:
        return out.getvalue()


def _describe_project(project, write):
    write(0, '{}'.format(project))
    write()

    write(0, 'Project nodes:')
    for node in project.nodes:
        _describe_node(1, node, write)
    write()

    write(0, 'Cells:')
    for cell in project:
        _describe_cell(cell, write)
    write()

    write(0, 'Meteo Stations:')
    for meteo in project.meteo_stations:
        _describe_meteo(meteo, write)
    write()

    write(0, 'Rain Stations:')
    for rain in project.rainfall_stations:
        _describe_rain(rain, write)


def _describe_timeseries(ts):
    """
    Describes a cmf timeseries with its start, end, step, min, mean and max.

    :param: ts: a cmf.timeseries
    :return: None
    """
    if ts:
        return ('{count} values from {start:%Y-%m-%d} to {end:%Y-%m-%d} step '
                '{step}, min/mean/max  {min:0.5g} / {mean:0.5g} / {max:0.5g}'
                .format(count=len(ts),
                        start=ts.begin,
                        end=ts.end,
                        step=ts.step,
                        min=ts.min(),
                        mean=ts.mean(),
                        max=ts.max()
                        )
                )
    else:
        return '~'  # With no data in the timeseries return YAML-NULL symbol


def _describe_node(indent_level, node, write):
    write(indent_level, '- {}:'.format(node))
    for connection in node.connections:
        write(indent_level + 1, '- {}'.format(connection))


def _describe_cell(cell, write):
    write(1, '- {}:'.format(cell))
    for storage in cell.storages:
        _describe_node(2, storage, write)


def _describe_meteo(meteo, write):
    write(1, '- {}:'.format(meteo))
    for var_name, timeseries in meteo.TimeseriesDictionary().items():
        write(2, '{}: {}'.format(var_name, _describe_timeseries(timeseries)))


def _describe_rain(rainstation, write):
    write(1, '- {} ({:0.2f}mm/year)'.format(rainstation.name,
                                            rainstation.data.mean() * 365))
    write(2, '{}'.format(_describe_timeseries(rainstation.data)))
