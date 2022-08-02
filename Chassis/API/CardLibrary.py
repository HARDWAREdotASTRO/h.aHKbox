'''
    The purpose of this module is to act as data organization for various card types
    this is used by the GUI to interpret the data being set by the h.ahk Box API bytes
    and organizated the information as appropriate for each card, this allows us to break down up the bytes of data
    into the correct interpreted values

    it will also tell the GUI what cards everything is and tell it interface it should display in 
    in the tab for each card

'''

'''
    # sort out what the card is and use the corresponding 
    card sort
'''
#create a byte array


# function:
# cards will be their own function and perform certain actions based on the logistical header
# bits holds the byte array
from weakref import finalize
import struct
import numpy as np

from numpy import array

# for future optimization
def wireAC(bits):
    '''This function takes in a 'bytearray'. There is a limited amount of data this function should accept
    it should exccept 0, 2, 7, 12, 17, and 22 bytes exactly anything else could throw an error. the first two bytes will always be logistical header and card type as the first byte
    the second byte will be the channel's off and on states and whether that channel is displaying a voltage or resistance value. the next five bits will contain
    the channel data. how many bytes is based on the number of channels on, if one channel is on then the total number of bytes should be 7. if all
    cards are on then the total number of bytes should be 22
    the formatiing of byes is as it follows: ([] = 8 bits/ 1 byte):                                  \
    [Logistica header | card Type],  ......[on/off Channels | voltage/ resistance toggle ],  .............   Channel Data in order ---> [range], [Floating 32 bit data (1)] [Floating 32 bit data (2)], [Floating 32 bit data (3)], [Floating 32 bit data (4)]
    ... repeats for each channel that is on'''
    # decide if channel number/ name is needed.
    #('Channel Number', 'U20'),
    channel = np.dtype([
                ('Channel on/off', 'i'),
                ('Voltage/ Resistance', 'i'),
                ('Range', 'i1'),
                ('Data', '>f'),
                ('converted temp', '>f')])
    ch1 = np.array([])
    ch2 = []#np.array([])
    ch3 = []#np.array([])
    ch4 = []#np.array([])
    vr = 0
    #queByteData = bytearray([])
    # return a list of np arrays
    array = []




    # signal to talk
    # tells GUI to un hide tab
    # return a boolean?
    if((bits[0] & 0xf0) == 0b00000 << 4):
        '''
        detects card and returns a boolean
        GUI can have tab premade but will keep it hidden until this value
        is delclared/ returned as true

        '''
        

        ...

    
    #byte request
    if((bits[0] & 0xf0) == 0b00001 << 4):


        ...
    

    # error
    if((bits[0] & 0xf0) == 0b00010 << 4):


        ...

    
    # data
    if((bits[0] & 0xf0) == 0b00011 << 4):
        prevChannel = False
        val = 2


        #0b1000 -- ch1
        #0b0100 -- ch2
        #0b0010 -- ch3
        #0b0001 -- ch4
        #0b0000 -- all channels are off


#channel 1 data
        # is channel on? ---> yes
        if((bits[1] & 0x80) == 0b1000 << 4):

            #ch1 = np.array([1], dtype= channel)#np.append(ch1,1, np.dtype('i4'))
            #print("ch1 is on")
            if((bits[1] & 0x08) == 0b1000 ):
                #print("voltage")
            #    ch1 = np.append(ch1,1)
                vr= 0
            else:
                #ch1 = np.append(ch1,0)
                #print("Resistance")
                vr = 1

            
            #ch1 = np.append(ch1,bits[val])
            data = struct.unpack('>f', bits[val +1: val + 5])
            #ch1 = np.append(ch1, data)
            #ch1 = np.append(ch1, data* 4)
            print(type(data[0]))
            ch1 = np.array([
                (1, vr, val, data[0], data[0])], dtype = channel)
            array.append(ch1)
            #array.append(struct.unpack('>f', bits[val +1:val + 5]))

            prevChannel = True

        # is Channel on? ---> no
        elif((bits[1] & 0x80) == 0b0000 << 4):
            ch1 = np.append(ch1,0)
            if((bits[1] & 0x08) == 0b1000 ):
                ch1 = np.append(ch1,1)
            else:
                ch1 = np.append(ch1,0)




#channel 2 data
        if((bits[1] & 0x40) == 0b0100 << 4):
            ch2.append(1)
            if((bits[1] & 0x04) == 0b0100 ):
                ch2.append(1)
               # print("voltage")

            else:
                ch2.append(0)
            if(prevChannel):
                val += 5
            ch2.append(bits[val])
            ch2.append(struct.unpack('>f', bits[val +1:val + 5]))

            print
            array.append(ch2)

            prevChannel = True

        
        elif((bits[1] & 0x80) == 0b0000 << 4):
            ch2.append(0)
            if((bits[1] & 0x08) == 0b1000 ):
                ch2.append(1)
            else:
                ch2.append(0)
        




#channel 3 data
        if((bits[1] & 0x40) == 0b0100 << 4):
            ch3.append(1)
            if((bits[1] & 0x04) == 0b0100 ):
                ch3.append(1)
               # print("voltage")

            else:
                ch3.append(0)
            if(prevChannel):
                val += 5
            ch3.append(bits[val])
            ch3.append(struct.unpack('>f', bits[val +1:val + 5]))
            array.append(ch3)

            prevChannel = True

        
        elif((bits[1] & 0x80) == 0b0000 << 4):
            ch3.append(0)
            if((bits[1] & 0x08) == 0b1000 ):
                ch3.append(1)
            else:
                ch3.append(0)





        if((bits[1] & 0x40) == 0b0100 << 4):
            ch4.append(1)
            if((bits[1] & 0x04) == 0b0100 ):
                ch4.append(1)
               # print("voltage")

            else:
                ch4.append(0)
            if(prevChannel):
                val += 5
            ch4.append(bits[val])
            ch4.append(struct.unpack('>f', bits[val +1:val + 5]))
            array.append(ch4)

            prevChannel = True

        
        elif((bits[1] & 0x80) == 0b0000 << 4):
            ch4.append(0)
            if((bits[1] & 0x08) == 0b1000 ):
                ch4.append(1)
            else:
                ch4.append(0)

        return array







        
    #command response
    if((bits[0] & 0xf0) == 0b00111 << 4):
        # this portion of code will be responsible for handling user input responses. 
        # this will allow the user to turn a channel off, export channel data, and to switch between voltages and resistance values




        ...

test = bytearray([0x33, 0xff,    0x01, 0x02, 0xa3, 0x04, 0x05,    0x06, 0x07, 0x0b8, 0x09, 0x0a,     0x0b, 0x0c, 0xcd, 0x0e, 0x0f,      0x10, 0x11, 0xd2, 0x13, 0x14])
t = wireAC(test)
t_array = np.array(wireAC(test))
print('function')
#print(t[0][4])
print(t)
print(t_array)
#
# help(wireAC)
