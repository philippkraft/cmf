

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
"""
The dbfreader and dbfwriter are written by Raymond Hettinger 
http://code.activestate.com/recipes/362715/ 
but slightly adopted (default return of float instead only decimals)
All other function by Philipp Kraft
"""
import struct, datetime, decimal, itertools

def dbfreader(f,numberAsFloat=True):
    """Returns an iterator over records in a Xbase DBF file.

    The first row returned contains the field names.
    The second row contains field specs: (type, size, decimal places).
    Subsequent rows contain the data records.
    If a record is marked as deleted, it is skipped.

    File should be opened for binary reads.

    """
    # See DBF format spec at:
    #     http://www.pgts.com.au/download/public/xbase.htm#DBF_STRUCT

    numrec, lenheader = struct.unpack('<xxxxLH22x', f.read(32))
    numfields = (lenheader - 33) // 32

    fields = []
    for fieldno in xrange(numfields):
        name, typ, size, deci = struct.unpack('<11sc4xBB14x', f.read(32))
        name = name.replace('\0', '')       # eliminate NULs from string
        fields.append((name, typ, size, deci))
    yield [field[0] for field in fields]
    yield [tuple(field[1:]) for field in fields]

    terminator = f.read(1)
    assert terminator == '\r'

    fields.insert(0, ('DeletionFlag', 'C', 1, 0))
    fmt = ''.join(['%ds' % fieldinfo[2] for fieldinfo in fields])
    fmtsiz = struct.calcsize(fmt)
    for i in xrange(numrec):
        record = struct.unpack(fmt, f.read(fmtsiz))
        if record[0] != ' ':
            continue                        # deleted record
        result = []
        for (name, typ, size, deci), value in itertools.izip(fields, record):
            if name == 'DeletionFlag':
                continue
            if typ == "N":
                value = value.replace('\0', '').lstrip()
                if value == '':
                    value = 0
                elif deci:
                    if numberAsFloat:
                        value=float(value)
                    else:
                        value = decimal.Decimal(value)
                else:
                    value = int(value)
            elif typ == 'D':
                y, m, d = int(value[:4]), int(value[4:6]), int(value[6:8])
                value = datetime.date(y, m, d)
            elif typ == 'L':
                value = (value in 'YyTt' and 'T') or (value in 'NnFf' and 'F') or '?'
            result.append(value)
        yield result


def dbfwriter(f, fieldnames, fieldspecs, records):
    """ Return a string suitable for writing directly to a binary dbf file.

    File f should be open for writing in a binary mode.

    Fieldnames should be no longer than ten characters and not include \x00.
    Fieldspecs are in the form (type, size, deci) where
        type is one of:
            C for ascii character data
            M for ascii character memo data (real memo fields not supported)
            D for datetime objects
            N for ints or decimal objects
            L for logical values 'T', 'F', or '?'
        size is the field width
        deci is the number of decimal places in the provided decimal object
    Records can be an iterable over the records (sequences of field values).

    """
    # header info
    ver = 3
    now = datetime.datetime.now()
    yr, mon, day = now.year-1900, now.month, now.day
    numrec = len(records)
    numfields = len(fieldspecs)
    lenheader = numfields * 32 + 33
    lenrecord = sum(field[1] for field in fieldspecs) + 1
    hdr = struct.pack('<BBBBLHH20x', ver, yr, mon, day, numrec, lenheader, lenrecord)
    f.write(hdr)

    # field specs
    for name, (typ, size, deci) in itertools.izip(fieldnames, fieldspecs):
        name = name.ljust(11, '\x00')
        fld = struct.pack('<11sc4xBB14x', name, typ, size, deci)
        f.write(fld)

    # terminator
    f.write('\r')

    # records
    for record in records:
        f.write(' ')                        # deletion flag
        for (typ, size, deci), value in itertools.izip(fieldspecs, record):
            if typ == "N":
                value = str(value).rjust(size, ' ')
            elif typ == 'D':
                value = value.strftime('%Y%m%d')
            elif typ == 'L':
                value = str(value)[0].upper()
            else:
                value = str(value)[:size].ljust(size, ' ')
            assert len(value) == size
            f.write(value)

    # End of file
    f.write('\x1A')

def dbflist(filename) :
    """Opens the dbf file and returns three lists containing the dbf data

    Returns:
        (fieldnames[0..n-1],fieldtypes[0..n-1],records[0..r-1][0..n-1]),
        where n is the number of fields and r is the number of records
    """
    f=file(filename,'rb')
    dbf=dbfreader(f)
    l=list(dbf)
    f.close()
    return l[0],l[1],l[2:]
def objects(classname,fieldnames,data):
    str = "class %s:\n    def __init__(self,params):\n" % classname
    for i,f in enumerate(fieldnames):
        str+=" " * 8 + "self.%s=params[%i]\n" % (f,i)
    exec(str)
    c=eval(classname)
    result=[]
    for line in data:
        result.append(c(line))
    return result
    
def dbfclass(classname,filename):
    """Returns a list of object of a class tailored with the fields as attributes"""
    fields,types,data=dbflist(filename)
    return objects(classname, fields, data)
def txtclass(classname,filename,seperator='\t',decimalpoint='.'):
    fields,data=txtlist(filename, seperator, decimalpoint) 
    return objects(classname,fields,data)
def txtlist(filename,seperator='\t',decimalpoint='.'):
    """ Opens a tab or other character seperated txt file and returns a list of fieldnames and a list of data"""
    f=file(filename)
    line=f.readline().rstrip()
    fieldnames=line.split(seperator)
    data=[]
    for line in f:
        items=line.split(seperator)
        record=[]
        for item in items:
            try:
                record.append(int(item))
            except:
                try:
                    record.append(float(item))
                except:
                    record.append(item)
        data.append(record)
    f.close()
    return fieldnames,data

            
    
    
    
    