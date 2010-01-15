

# Copyright 2010 by Philipp Kraft
# This file is part of cmf.
#
#   cmf is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 2 of the License, or
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
import cmf
import DWD

def GetMeteorology(cmf_project,dwdBestandFile,dwdSatzFile,dwdDatFile,start,end,daily=1,removeNoData=0):
    """ Returns a dictionary of cmf.Meterology indexed by the id of the station
    
    dwdBestandFile Filename of the DWD file 'Bestand'
    dwdSatzFile    Filename of the DWD file 'Satz'
    dwdDatFile     Filename of the DWD file 'Dat'
    start          cmf.Time of the beginning of the time of interest
    end            cmf.Time of the end of the time of interest
    daily          Boolean value, if true the radiation procedure does not generate diurnal changes
    removeNoData   Boolean value, if true NoData will be overridden by the preceeding value, 
                   if false the timeseries will start after the data gap 
    """
    #Load the stations from the DWD files
    stations=DWD.LoadDWD(dwdBestandFile,dwdSatzFile,dwdDatFile)
    return convertstations(cmf_project,stations, start, end, daily, removeNoData)
def convertstations(cmf_project,stations,start,end,daily=1,removeNoData=0):
    # The dictionary, that will be returned
    meteorology={}
    
    for st in stations.values() :
        print st.number,
        # Create the meteorology
        meteo=cmf_project.meteo_stations.add_station(st.number,st.lat,st.lon,1,st.height,start,cmf.day)
        meteo.daily=daily
        # Get the dictionary of column positions
        columndict=st.ColumnHeaderDict()
        # Get the dictionary of records, indexed by datetime
        datadict=st.DataAsDictionary()
        # Get the dictionary of timeseries of the meteo. Make sure the column headers and the keys of the timeseries match
        # Key of the timeseries are the names of the variable holding the timeseries
        tsdict=meteo.TimeseriesDictionary()
        # Well, start at start
        curdate=start
        # A variable to hold the current record
        r=None
        nodatacount=0
        datacount=0
        while curdate<=end :
            # Convert cmf.Time to datetime
            pydate=curdate.AsPython().date()
            # If curdate is in the station dataset
            if pydate in datadict :
                
                # Read the record at curdate
                r=datadict[pydate]
                datacount+=1
                # For each column name
                for col in columndict:
                    # Add the value at record r and column col to the timeseries col (only if tsdict[col] exist, of course)
                    if col in tsdict : tsdict[col].add(r[columndict[col]])
            else : # curdate has no data
                # Last loop values where retrieved and remove no data
                if r and removeNoData :
                    # Use the data of the last loop 
                    nodatacount+=1
                    datacount+=1
                    for col in columndict: 
                        # Add the value at record r and column col to the timeseries col (only if tsdict[col] exist, of course)
                        if col in tsdict : tsdict[col].add(r[columndict[col]])
                else: # Whether r was never established, or no data areas are to be removed       
                 # Remove data from all timeseries and set the begin on tomorrow
                 for ts in tsdict.values():
                     ts.clear()
                     ts.begin=curdate+cmf.day
            curdate+=cmf.day
        meteo.SetSunshineFraction(meteo.Sunshine)

        print " %i records imported, %i no data records estimated" % (datacount,nodatacount)
        meteorology[st.number]=meteo
    return meteorology
def get_rainfall(data_filename):
    f=file(data_filename)
    timeseries={}
    for line in f:
        id=line[0:5]
        year=int(line[5:9])
        month=int(line[9:11])
        day=int(line[11:13])
        rr=int(line[14:18])
        timeseries.setdefault(id, cmf.timeseries(cmf.Time(day,month,year),cmf.day)).add(rr*0.1)
    return  timeseries
        
