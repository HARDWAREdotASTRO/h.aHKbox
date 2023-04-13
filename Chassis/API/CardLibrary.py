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

import struct
import numpy as np

from numpy import array



# future note: turn into a class object instead
def cardType(header):
    ''' keep in mind this function is a prototype and that this is a long way to parse out data. I would recomment find a shorter
    and possibly much more efficent way to parse and sort this data properly. n\
    This function could also be done as a class instead but for now I wrote it as a function. it takes in data from the h.ahk box
    and looks at the first header data. it reads the card type bits and then sends the rest of the data to be read and interpreted by
    its corresponding card type function'''

    if((header[0] & 0x0f) == 0x01):
        #card type function
        print("Card type 1")
        
    if((header[0] & 0x0f) == 0x02):
        # Card type 2 function
        print("Card type 2")
        
    if((header[0] & 0x0f) == 0x03):
        print("----Card type 3-----")
        out = wireAC(header)
        
    if((header[0] & 0x0f) == 0x04):
        # Card Type 4 function
        print("Card type 4")
        
    if((header[0] & 0x0f) == 0x05):
        # Card type 5 function
        print("Card type 5")
    return out       


# for future optimization
def wireAC(bits):
    '''This function takes in a 'bytearray'. There is a limited amount of data this function should accept
    it should exccept 0, 2, 7, 12, 17, and 22 bytes exactly anything else could throw an error. the first two bytes will always be logistical header and card type as the first byte
    the third byte will be the channel's off and on states and whether that channel is displaying a voltage or resistance value. the next five bits will contain
    the channel data. how many bytes is based on the number of channels on, if one channel is on then the total number of bytes should be 7. if all
    cards are on then the total number of bytes should be 22
    the formatiing of bytes is as it follows: ([] = 8 bits/ 1 byte):                                  
    [Logistical header | card Type],  ...... [on/off Channels | voltage/ resistance toggle ],  .............   Channel Data in order ---> [range], [Floating 32 bit data (1)] [Floating 32 bit data (2)], [Floating 32 bit data (3)], [Floating 32 bit data (4)]
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
    ch2 = np.array([])
    ch3 = np.array([])
    ch4 = np.array([])
    #either a 1 or a 0 depending on if the value is a voltage or resistance
    vr = 0
    #queByteData = bytearray([])
    # return a list of np arrays
    array = []




    # signal to talk
    # tells GUI to generate a 4 wire AC tab interface
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
            print("Channel one is selected!")

            # returns if its a voltage  or a resistance
            if((bits[1] & 0x08) == 0b1000 ):

                vr = 0
            else:

                vr = 1
            #unpacks byte value
            data = struct.unpack('>f', bits[val + 1: val + 5])
            print(data)

            print(type(data[0]))
            ch1 = np.array([
                (1, vr, val, data[0], data[0])], dtype = channel)
            array.append(ch1)
            # boolean is to let other channels know the one before it was on.
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
            print("Channel two is selected!")

            if((bits[1] & 0x04) == 0b0100 ):

                vr = 0
            else:

                vr = 1
            # boolean variable is that it will know if the previous channel was on
            #it will shift the value to the next 5 bits of information
            if(prevChannel):
                val += 5 

            data = struct.unpack('>f', bits[val + 1: val + 5])
            print(data)

            print(type(data[0]))
            ch2 = np.array([
                (1, vr, val, data[0], data[0])], dtype = channel)
            array.append(ch2)

            prevChannel = True

        # is Channel on? ---> no
        elif((bits[1] & 0x40) == 0b0000 << 4):
            ch2 = np.append(ch2,0)
            if((bits[1] & 0x04) == 0b0100 ):
                ch2 = np.append(ch2,1)

            else:
                ch2 = np.append(ch2,0)
        '''
        Simply keeps bit values and checks if the off channel would have been a resistance or voltage 
        As of 3-24-23 unsure if this section is really needed?
        '''
        




#channel 3 data
        if((bits[1] & 0x20) == 0b0010 << 4):
            print("Channel three is selected!")

            if((bits[1] & 0x02) == 0b0010 ):

                vr = 0
            else:

                vr = 1
            if(prevChannel):
                val += 5 

            data = struct.unpack('>f', bits[val +1: val + 5])
            print(data)

            print(type(data[0]))
            ch3 = np.array([
                (1, vr, val, data[0], data[0])], dtype = channel)
            array.append(ch3)

            prevChannel = True

        # is Channel on? ---> no
        elif((bits[1] & 0x20) == 0b0000 << 4):
            ch3 = np.append(ch3,0)
            if((bits[1] & 0x02) == 0b0010 ):
                ch3 = np.append(ch3,1)

            else:
                ch3 = np.append(ch3,0)
        





    # channel 4
        if((bits[1] & 0x10) == 0b0001 << 4):
            print("Channel four is selected!")

            if((bits[1] & 0x01) == 0b0001 ):

                vr = 0
            else:

                vr = 1
            if(prevChannel):
                val += 5 

            data = struct.unpack('>f', bits[val +1: val + 5])
            print(data)

            print(type(data[0]))
            ch4 = np.array([
                (1, vr, val, data[0], data[0])], dtype = channel)
            array.append(ch4)

            prevChannel = True

        # is Channel on? ---> no
        elif((bits[1] & 0x10) == 0b0000 << 4):
            ch4 = np.append(ch4,0)
            if((bits[1] & 0x01) == 0b0001 ):
                ch4 = np.append(ch4,1)

            else:
                ch4 = np.append(ch4,0)
    return ch1,ch2,ch3,ch4
        

  






        
    #command response
    if((bits[0] & 0xf0) == 0b0111 << 4):
        # this portion of code will be responsible for handling user input responses. 
        # this will allow the user to turn a channel off, export channel data, and to switch between voltages and resistance values




        ...

# below the 4 wire ac card more card information
    # def cardError():
    #     ...
# this below here is old testing code. this will need to be removed
test = bytearray([0x33, 0xff,    0x01, 0x02, 0xa3, 0x04, 0x05,    0x06, 0x07, 0x0b8, 0x09, 0x0a,     0x0b, 0x0c, 0xcd, 0x0e, 0x0f,      0x10, 0x11, 0xd2, 0x13, 0x14])

test2 = bytearray([0x33, 0x88,    0x01, 0x02, 0xa3, 0x04, 0x05])
t = wireAC(test)
#t_array = np.array(wireAC(test))
print('function')
#print(t['Channel on/off'])
#print(t[0][4])
#find a way to return value
#since now  it is not returning anything
#make sure it is ablee to find the value and returns something
print(t)
#print(t_array)
#
# help(wireAC)
