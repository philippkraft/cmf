from datetime import *
import os
from numpy import array

class Station:
    """Holds the data and the metadata of a meteorological station
    Metadata:
        number : Value representing the Station id (usually a string)
        name   : The name of the meteorological Station
        lon    : Longitude in dec. degrees
        lat    : Latitude in dec. degrees
        height : Height above sea level in m
    Data:
        columnheads : List of strings describing the values stored in data 
        data        : List of records. Records are a list of doubles, containing for each columnhead the value
        days        : Date of each record. Must have the same length as data
    Methods:
        begin()               : returns date of first record
        end()                 : returns date of last record
        GetParts()            : returns the parts of the timeseries (non continous dates)
        SetColumnHeaders()    : sets the columnheads from a columns dictionary (see GetDataDictionary)
        GetColumnHeadersDict(): returns a dictionary translating a column name to the index of the column
        InterpolateNoData()   : Interpolates no data with the last and the next value. Does not work for no data at the edges of the time series  
    """
    def __init__(self,number="",name="",lon=0.,lat=0.,height=0.):
        self.number=number
        self.name=name
        self.lon=lon
        self.lat=lat
        self.height=height
        self.data=[] # List of lists of double self.data[i]=Values for column heads at day self.days[i]
        self.days=[] # List of dates
        self.columnheads=[] # List of strings, contains the column headers
    def begin(self):
        return self.days[0]
    def end(self):
        return self.days[-1]
    def SetColumnHeaders(self,columns):
        self.columnheads=[]
        for col in columns:
            if col not in ['id','d','y','m']:
                self.columnheads.append(str(col))
    def ColumnHeaderDict(self):
        res={}
        for i,col in enumerate(self.columnheads):
            res[col]=i
        return res  
    def DataAsDictionary(self):
        """Creates a dictionary with dates as key and a record of self.data as value"""
        data={}
        for i,r in enumerate(self.data):
            data[self.days[i]]=r 
        return data
    def InterpolateNoData(self):
        for i,r in enumerate(self.data) :
            for j,v in enumerate(r):
                if v==-999 or v==-99 :
                    try:
                        self.data[i][j]=self.data[i-1][j]
                    except IndexError:
                        pass
    def __str__(self):
        return '%5s:%-24s%2.2fN,%2.2fO,%0.0fm\n%5ix%s' % (self.number,self.name,self.lat,self.lon,self.height,len(self.data),self.columnheads) 

                

def __getFromString(s,col_tuple):
    return s[col_tuple[0]-1:col_tuple[1]-1]
def GetStations(filenameBestand):
    """Returns a dictionary of stations (key: Station id) loaded from a DWD file 'bestand' """
    #Columns of station description
    no_col=(11,16) # Number
    name_col=(17,41) # Name
    lat_deg_col=(71,73) # Longitude degrees
    lat_min_col=(74,76) # Longitude minutes
    lon_deg_col=(80,82) # Latitude degrees
    lon_min_col=(84,86) # Latitude minutes
    h_col=(95,100)      # Height
    lineprefix=' Station='     # Keyword for beginning of lines with station description
    stations={}
    fBestand=file(filenameBestand)
    for line in fBestand:
        if line.startswith(lineprefix) :
           station=Station()
           station.number=__getFromString(line, no_col)
           station.name=__getFromString(line, name_col).strip()
           station.lon=float(__getFromString(line, lon_deg_col))
           station.lon+= float(__getFromString(line, lon_min_col))/60
           station.lat=float(__getFromString(line, lat_deg_col)) + float(__getFromString(line, lat_min_col))/60
           station.height=float(__getFromString(line, h_col))
           stations[station.number]=station
    fBestand.close()
    return stations
                
def GetDataColumns(filenameSatz):
    """Gets columns and conversion functions for a DWD 'dat' file from a DWD 'satz' file
    
    It is quite possible, that this function has to be adjusted to your needs.
    The kewords dictionary lists keywords from the 'satz' file to identify the position of data in the 'dat' file.
    If a value is not found, feel free to extend the dictionary. If other conversions than given are needed, extend the 
    conversions dictionary
    """
    
    def getTuple(line): # Only internally used, reads the column position in the 'dat' file from the 'satz' file
        return int(line[12:17]),int(line[19:23])+1
    fSatz=file(filenameSatz)
    # Extend the list of keywords if ysou encounter new description textes in the DWD "Satz" files
    keywords={"STATIONSNUMMER":"id","JAHR":"y","MONAT":"m","TAG  ":"d",
              "TAEGL.MAXIMUM DER LUFTTEMP.":"Tmax","TAEGL.MINIMUM DER LUFTTEMP.":"Tmin",
              "REL. FEUCHTE TAGESMITTEL":"rHmean","TAGESMITTEL DER WINDSTAERKE":"Windspeed","TAEGL. SONNENSCHEINDAUER":"Sunshine",
              "TAEGL.NIEDERSCHLAGSHOEHE":"Prec","TAGESSUMME DES NIEDERSCHLAGS":"Prec"}
    columns={}
    # Extent the dictionary of conversions, if you need additional conversions. Use lambda functions for the conversions
    conversions={"Tmax":lambda t:t*0.1,"Tmin":lambda t:t*0.1,   # 0.1 deg C -> deg C
                 "Prec":lambda p:p*0.1,                         # 0.1 mm    -> mm
                 "Windspeed":lambda ws:0.836*(ws*0.1)**1.5,     # 0.1 Bft   -> m/s 
                 "Sunshine":lambda s:s*0.1,                     # 1/10 h    -> h
                 "Clouds":lambda c:c/8.0}
    # Adds the columns for the data file with the keywords above
    for i,line in enumerate(fSatz): # Reads every line in 'satz'
        if i>12:                    # Ignore the first 12 lines (only explantions)
            description=line[35:78] # Find the description
            for key in keywords:    # Test if one of the keywords is in the description 
                if key in description :
                    # Add the shorter key word and the position of the value in the 'dat' file to the dictionary
                    columns[keywords[key]]=getTuple(line) 
    fSatz.close()
    return columns,conversions

def AddStationColumnheads(columns,stations):
    """Adds the column heads from GetDataColumns to each station from GetStations"""
    for s in stations.values():
        s.SetColumnHeaders(columns)
def GetData(filenameData,stations,columns,conversions):
    """Loads the data in the 'dat' file in each station from GetStations() using the metadata and conversions from GetDataColumns()""" 
    fData=file(filenameData)
    lastId=''
    for i,line in enumerate(fData):
        #Gets the station id
        id=__getFromString(line, columns['id'])
        try:
            if id!=lastId : print id,
            lastId=id
            station=stations[__getFromString(line, columns['id'])]
        except  KeyError:
            print id, ' not found ',
        # Get the date
        y,m,d=(int(__getFromString(line, columns['y'])),
               int(__getFromString(line, columns['m'])),
               int(__getFromString(line, columns['d'])))
        try:
            station.days.append(date(y,m,d))
        except ValueError :
            print "Not a date: %i.%i.%i" % (d,m,y)
        #Get record
        record=[]
        for col in station.columnheads :
            val=int(__getFromString(line, columns[col]))
            if col in conversions and val!=-999 and val!=-99 :
                val=conversions[col](val)
            record.append(val)
        station.data.append(record)
    print
    fData.close()
def LoadDWD(filenameBestand,filenameSatz,filenameData):
    stations=GetStations(filenameBestand)
    columns,conversions=GetDataColumns(filenameSatz)
    print "Columns: ",columns.keys() 
    AddStationColumnheads(columns, stations)
    print "Columns added to stations..."
    GetData(filenameData, stations, columns, conversions)             
    for st in stations.values() :
        st.InterpolateNoData()
        print st    
    return stations      
   
            
if __name__=="__main__" :
    dir=r"d:\Philipp\Daten\Eichelsachen\climate"
    curdir=os.curdir
    os.chdir(dir)
    stations=LoadDWD('kl_bestand_abgabe440_2', 'kl_satz_abgabe440_2', 'kl_dat_abgabe440_2')
    os.chdir(curdir)
        
            
            
    

                      
                    

                
                
                

                
            
        
        
        
        
        
        