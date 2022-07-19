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
def wireAC(bits):
    ch1 = 0
    ch2 = 0
    ch3 = 0
    ch4 = 0
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
        #0b1000 -- ch1
        #0b0100 -- ch2
        #0b0010 -- ch3
        #0b0001 -- ch4
        #0b0000 -- all channels are off
        if((bits[1] & 0x80) == 0b1000 << 4):
            print("ch1 is on")
            if((bits[1] & 0x08) == 0b1000 ):
                print("voltage")
            else:
                print("Resistance")


        if((bits[1] & 0x40) == 0b0100 << 4):
            print("ch2 is on")
            if((bits[1] & 0x04) == 0b0100 ):
                print("voltage")
            else:
                print("Resistance")
            print("range: " + bits[2])
            



        if((bits[1] & 0x20) == 0b0010 << 4):
            print("ch3 is on")
            if((bits[1] & 0x02) == 0b0010 ):
                print("voltage")
            else:
                print("Resistance")
            
    
        if((bits[1] & 0x10) == 0b0001 << 4):
            print("ch4 is on")
            if((bits[1] & 0x01) == 0b0001 ):
                print("voltage")
            else:
                print("Resistance")

        return ch1, ch2, ch3, ch4







        
    #command response
    if((bits[0] & 0xf0) == 0b00111 << 4):
        # this portion of code will be responsible for handling user input responses. 
        # this will allow the user to turn a channel off, export channel data, and to switch between voltages and resistance values




        ...

test = bytearray([0x33, 0xff, 0x02, 0x78, 0xdf, 0x3a, 0x55])
wireAC(test)
