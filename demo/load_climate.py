import cmf
import pandas as pd


def load_climate_data(project, climate_file):
    """
    Loads climate data from 1980 to 2006 for Giessen from
    a csv file and adds it to a cmf project as a meteorological station
    and a rain station.
    Data provided by DWD

    :returns: meteo station, rainfall station
    """
    data = pd.read_csv(climate_file, parse_dates=['date'], index_col=0)

    meteo = project.meteo_stations.add_station(
        'giessen', position=(0, 0), latitude=51
    )
    begin = data.index[0].to_pydatetime()

    def col2ts(column_name):
        return cmf.timeseries.from_array(begin, cmf.day, data[column_name])

    meteo.Tmin = col2ts('T min [degC]')
    meteo.Tmax = col2ts('T max [degC]')
    meteo.rHmean = col2ts('rH mean [%]')
    meteo.Sunshine = col2ts('sunshine [h/h]')
    meteo.Windspeed = col2ts('windspeed [m/s]')

    rainstation = project.rainfall_stations.add(
        'Giessen',
        col2ts('Prec [mm/day]'),
        Position=(0, 0, 0)
    )

    return meteo, rainstation
