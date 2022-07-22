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

# for future optimization
def wireAC(bits):
    ch1 = bytearray([])
    ch2 = bytearray([])
    ch3 = bytearray([])
    ch4 = bytearray([])
    queByteData = bytearray([])




    # signal to talk
    if((bits[0] & 0xf0) == 0b00000 << 4):
        

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
        if((bits[1] & 0x80) == 0b1000 << 4):
            print("ch1 is on")
            if((bits[1] & 0x08) == 0b1000 ):
                print("voltage")
                
                for i in range(5):
                    print(str(int(bits[i+val])))
                #print("voltage: " + str(struct.unpack('<f', bits[val:val + 4])))
                print("voltage: " + str(struct.unpack('>f', bits[val +1:val + 5])))

                prevChannel = True
            else:
                print("Resistance")


        if((bits[1] & 0x40) == 0b0100 << 4):
            print("ch2 is on")
            if((bits[1] & 0x04) == 0b0100 ):
                print("voltage")
                if(prevChannel):
                    val += 5
                for i in range(5):
                    print(str(int(bits[i+val])))
                prevChannel = True
            else:
                print("Resistance")
            print("range: " + str(bits[val-4]))
            



        if((bits[1] & 0x20) == 0b0010 << 4):
            print("ch3 is on")
            if((bits[1] & 0x02) == 0b0010 ):
                print("voltage")
                if(prevChannel):
                    val += 5
                for i in range(5):
                    print(str(int(bits[i+val])))
                prevChannel = True
            else:
                print("Resistance")
            
    
        if((bits[1] & 0x10) == 0b0001 << 4):
            print("ch4 is on")
            if((bits[1] & 0x01) == 0b0001 ):
                print("voltage")
                #note: you do need this boolean value.
                #this boolean acts as a check to make sure the first 5 bytes are reached
                # if previous channel as alread toggled this than it should be act as normal
                # but say only channel 4 is on and if this boolean was not here it would immediately try to go to the next 5 bytes
                #rather than get the next bytes right after the inital 2 bytes in the given array
                if(prevChannel):
                    val += 5
                for i in range(5):
                    print(str(int(bits[i+val])))
                prevChannel = True
            else:
                print("Resistance")

        return ch1, ch2, ch3, ch4







        
    #command response
    if((bits[0] & 0xf0) == 0b00111 << 4):
        # this portion of code will be responsible for handling user input responses. 
        # this will allow the user to turn a channel off, export channel data, and to switch between voltages and resistance values




        ...

test = bytearray([0x33, 0x8f,    0x01, 0x02, 0xa3, 0x04, 0x05,    0x06, 0x07, 0x08, 0x09, 0x0a,     0x0b, 0x0c, 0x0d, 0x0e, 0x0f,      0x10, 0x11, 0x12, 0x13, 0x14])
wireAC(test)
