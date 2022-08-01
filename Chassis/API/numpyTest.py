import numpy as np
from CardLibrary import wireAC
import datetime


date = datetime.datetime.now()
voltRes = 'Resistance'
timearray = np.array(['Time'])
xarray = np.array(['channel 1'])
yarray = np.array(['channel 2'])
ch3array = np.array(['channel 3'])
ch4array = np.array(['channel 4'])
i = 0

test = bytearray([0x33, 0xff,    0x01, 0x02, 0xa3, 0x04, 0x05,    0x06, 0x07, 0x0b8, 0x09, 0x0a,     0x0b, 0x0c, 0xcd, 0x0e, 0x0f,      0x10, 0x11, 0xd2, 0x13, 0x14])
while i < 5:

    timearray = np.append(timearray, date)
    xarray = np.append(xarray, wireAC(test)[0])
    yarray = np.append(yarray, wireAC(test)[1])
    ch3array = np.append(ch3array, wireAC(test)[2])
    ch4array = np.append(ch4array, wireAC(test)[3])
    i = i + 1


# here is your data, in two numpy arrays

data = np.column_stack([xarray, yarray, ch3array, ch4array,])
datafile_path = "/Users/hy2755lw/Dropbox/4WireAC.txt "

np.savetxt(datafile_path , data, fmt=['%d','%d','%d','%d'])
# here the ascii file is written. 

#file = np.savetxt("4AC-ch1.txt", ch1, fmt='%s')
a = open(datafile_path, 'r')
print(a.read())