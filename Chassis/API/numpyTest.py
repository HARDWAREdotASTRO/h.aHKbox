import numpy as np
from CardLibrary import cardType
#reload(cardType)
import datetime


date = datetime.datetime.now()
TodaysDate = str(datetime.datetime.now().month) + str(datetime.datetime.now().day) + str(datetime.datetime.now().year)
voltRes = 'Resistance'
timearray = np.array(['Time'])
cardarray = np.array(['Card_Type'])
temparray = np.array(['Temperature'])
xarray = np.array(['channel 1'])
yarray = np.array(['channel 2'])
ch1array = np.array(['channel 1'])
ch2array = np.array(['channel 2'])
ch3array = np.array(['channel 3'])
ch4array = np.array(['channel 4'])
print(temparray)
#ch1array = np.array(['channel 2'])
tv = 1
i = 0
while i < 10:
    test = bytearray([0x33, 0xff,    0x01, tv, 0xa3, 0x04, 0x05,    0x06, 0x07, 0x0b8, 0x09, 0x0a,     0x0b, 0x0c, 0xcd, 0x0e, 0x0f,      0x10, 0x11, 0xd2, 0x13, 0x14])
    test = bytearray([0x33, 0x88,    0x01, 0x02, 0xa3, 0x04, 0x05])
    '''
    0011 0011  1111 1111
    0000 0001  0000 0001  1010 0011  0000 0100  0000 0101

    '''
    test = bytearray([0x33, 0xff,    0x01, 0x02, 0xa3, 0x04, 0x05,    0x06, 0x07, 0x0b8, 0x09, 0x0a,     0x0b, 0x0c, 0xcd, 0x0e, 0x0f,      0x10, 0x11, 0xd2, 0x13, 0x14])

    test2 = bytearray([0x33, 0x88,    0x01, 0x02, 0xa3, 0x04, 0x05])
    #test2 = bytearray([0x33, 0x88,    0x01, tv, 0xa3, 0x04, 0x05])
    t = cardType(test2)

    #adds date to arrsy
    timearray = np.append(timearray, str(date))
    cardarray = np.append(cardarray, str(test[0] & 0x0f))
    # temperature values
    print("testing to see the value \n")
    print(t[0][0])
    print(type(t[0]))
    temparray = np.append(temparray, str(t[0])+ '\n')
    ch1array = np.append(ch1array, str(t[0][0])+ '\n')
    ch2array = np.append(ch2array, str(t[0][0])+ '\n')
    
    ch3array = np.append(ch3array, str(t[0][0])+ '\n')
    ch4array = np.append(ch4array, str(t[0][0])+ '\n')

    # timearray = np.array(date)
    # cardarray = np.array(test[0] & 0x0f)
    # temparray = np.array( t)
    # ch1array = np.array( ch1array, t[0][0]+ '\n')
    # ch2array = np.array(ch2array, str(t[0][0])+ '\n')
    
    # ch3array = np.array(ch3array, str(t[0][0])+ '\n')
    # ch4array = np.array(ch4array, str(t[0][0])+ '\n')
    tv = 1

    #i = i + 1

    # here is your data, in two numpy arrays


    #remeber to add back the data
    data = np.column_stack([timearray, cardarray, temparray, ch1array, ch2array, ch3array, ch4array]) 
    #print(type(t[1][3]))
    '''
    an important thing to not here is this is a temporary file location 
    and will need to be changed to fit your computer destination!

    '''
    datafile_path = "/Users/hy2755lw/Dropbox/" + TodaysDate + "-4WireAC.txt"
    #a = open(datafile_path, 'a')
    a = open(datafile_path, 'a+')
    np.savetxt(datafile_path , data , delimiter= ',', fmt=['%s','%s','%s','%s','%s','%s','%s'])
    i = i+1
# here the ascii file is written. 

#file = np.savetxt("4AC-ch1.txt", ch1, fmt='%s')
#a = open(datafile_path, 'r')
print(a.read())
a.close()
