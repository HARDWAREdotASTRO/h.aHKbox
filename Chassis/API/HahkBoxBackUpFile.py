import time
import os
#import RPi.GPIO as GPIO
from struct import*
from smbus import SMBus
leader = 0x00
bus = SMBus(1)
followerResponse = 0x10 #0b0001 << 4
lRequest = 0x00 #0b0000 << 4
lResponse = 0x10#0b0001 << 4
cRequest = 0x70 #0b0111 << 4
cResponse = 0x80 #0b1000 << 4
interrupt = 0x50 #0b0101 << 4
boot = 8 #0x08 #0x60 #0b0110 << 4
error = 0x20 #0b0010 << 4
data = 0x30 #0b0011 << 4
ack = 0x40 #0b0100 << 4
shutdown = 0xe0 #0b1110 <<4 # change these bits later to fit the data sheet mroe acurately
#byteRequest = 0x80 #0b1000 << 4
dataRequest = 0xa0  #0b1010 << 4

valueMask = 0b01111111
flagBit = 0b10000000

#availableAddresses = []

'''
    note:
    add Follwer function that contains the name of the device so it can automatiicaly
    be read by the rasperrby pi and put its name in a drop box container
    or if the binary value of the card never changes you can have a script in the GUI program itself that will match the
    binary ID with its actual name and display that instead of its binary address id
'''


class Leader:
   # i2c = busio.I2C(board.SCL, board.SDA)

   #so far the 4 ac uses the most amount a bytes it uses 6 but the byte array may meed to be set at at higher
   #amount inorder to be used by Follwers that are going to be sending a larger amount of bytes (may set it 16 bytes sinces 32 bits shouldn't be needed)

   #use dataframe to help redeuce data frame sizes - use without it at first.
    dataFrame = bytearray(32)


    def __init__(self, address = 0x08):
        self.address = 0x08
       # self.leader = smbus.SMBus(1) 
  
        
        #used as default settings for the Leader. 
        #does the leader have anthing it needs to be defined as itself (maybe not, the leader is really only displaying values and request specific data 
        # from certain cards.)
        
        


    def read(self, address, byteRequest):
 
        dataFrame = bus.read_i2c_block_data(address, 0, byteRequest)


        return dataFrame



    def write(self, address, msg):



        bus.write_block_data(address, 0, msg)

    
    # note to self to replace time.sleep functions with a timeout counter


    def boot(self):
        #this will intialize all addresses the Leader in the firmware should have a prelist of some addresses (maybe the location of each card)
        availableAddresses = []
        # 10 addr
        i = 0
        
        # Note:
            # a constant int o '3' is being used to test the function
            # this int will need to be eventually changed so that 
            # it can loop through a max of 10 cards.
        
        
        #remember to uncomment continue in loop as well.. and to also intdent code
        while i <= 10:
            try:
                self.write(i, [i]) #12 was in place but changed it to the I value 
                response = [self.read(i, 1)]
           # response = bus.read_i2c_block_data(0x04, 0, 2)
                #print(response[0)


                print(response[0])
                temp = bytes(response[0])
                print(type(temp))
                print(type(bytes(0x10 | i)))
                print(bytes(temp))
                print(0x10 | i)
                print(bytes([0x10 | i]))
               # print(bin(lResponse&0xff))
                if temp == bytes([0x10 | i]): #int(response[0]) ==  0x13: #followerResponse | bin(i):
                    availableAddresses.append(i)
                    self.write(i, [ack | i])  # boot is sending Ack response to tell its been processes
                    print("Card slot "+str(i)+ " responded")
                    i = i + 1
                    continue
                else:
                    print("Invalid Response: \n Value Received: " + str(response[0]) +"\n Value Expected: " + bin(i| lResponse))
                    i = 1+i
                    continue
            except OSError:        
                time.sleep(.05)
                print('Card slot '+ str(i) + ' did not respond.  ')
                i = i + 1

        return availableAddresses


        #return an array of addresses
        #the poll function will be programmed in the actual firmware.
        
        
        
        
        
        
    '''
    05 - 16 - 2022
    
    
    errors to fix:
    bit mask for yesNo: value of bit mask does not change regardless of bitmask
    its recvieing a value of 20 which is 0010 0000: try and review arduino code and make sure it is sending a recieving values correctly
    
    '''
 
    def poll(self, addresses): #commented out variables 'me' 'address'
        #it wait 100ms for a response from follower
        #else it will time out and report an error
        # remove from addresses[]
      #  addresses = []
      #  addresses.append()
       # print(addresses) # print to mark when the poll function is active in data output i2c detect terminal command can be used with python to use for i2c
        #
        #if addresses:
        #while(True): #when in a while loop it does not repeat is it becasue of the return function?
        counter = 0
        counter = counter + 1
        for a in addresses:
           ## print("loop number: " + str(counter))
           
           # 6-17-18 using a special poll header of 0x5
            #print(0x50 | a)
            self.write(a, [ 0x50 | a] ) #(lRequest | address))
            time.sleep(.25)
            #print(bin(0xff & a))
            # will wait for a response from the card
            #if not response it should skip this card
            #if card does not respond a second time then to remove it from the system
            
            #self.wait(50)


            response = self.read(a, 2) #commented out addresses  where a currently is
           # print("first response back: " + str(response[1]))
            time.sleep(.25)
            # response byte 1 will contain header and card number, byte 2 contains value yes or no and how many bytes
             #   i = i +1;
            #
            #print(type(response))
          #  for i in response:
           #     print("response " + str(i) + "binary: " + bin(i))
            yesNo = response[1] #^ flagBit
            #print(yesNo)
           # print("yes /no value:" + str(yesNo) + " binary:" + bin(yesNo & 0xff) + " \n raw response [0]: \n" + str(response[0]) + " binary: " + bin(response[0] & 0xff)+ " \n raw response[1]: \n" + str(response[1]) + " binary: " + bin(response[1] & 0xff))
            # note: REMEMBER  to add a part of code to make sure its getting data from the right card
            #counter = counter + 1;
            #print("\n")
            if(yesNo < 128):
                print("information is being recieved..")
              #  print(response[0])
              #  print(response[1])
                byteRequest = response[1] 
                print(type(byteRequest))
                self.write(a,[0xa0 | a ]) # changed addresses[a] to just 'a' in this line it will write to the arduino the number of bytes it wants.
                information = unpack("BBBBBB", bytes(self.read(a, int(byteRequest) ))) # changed addresses[a] to just 'a' removed: response << 8 from this line
                print("data received from card Type " + str(a) + " binary: " + bin(a) + ": \n" + str(information) )
              
                time.sleep(.5)
                # ack knowledges that information hAS BEEN recieved
                self.write(a ,[a | ack])
                
                print(a | ack)
                #need multiple read functions, 
                #use the number of bytes to determine what read function to use.
                # add info for after the byte request
                print("\n")
               # print("\n")
                return information #/// commented out temp to see if its the problem for no loops note: not responsible for breaking the loop
        #counter = counter + 1
                
            if((yesNo & 0x80) == 128): #friendly reminder that 255 means empty slot. trying with both 255 and 0 see whihc works correctly
                print('no information!')
                print("response value \n" + str(response[1]))
                print("\n")
               # print("\n")
                return
            #counter = counter + 1   
            # time.sleep(.1)
            # print('no reponse after byte request')
            #remove from list
            #continue
        else:
            print("empty")
            
            
        #     print()


        time.sleep(1)
        #remove from list
        print('did not respond')
        





    def shutdown(self):
        addresses = []
        addresses.append(boot())

        #for loop for each item in addresses i

        self.write(addresses[i], addresses[i] | shutdown)
        response = self.read(addresses[i])
        # have card response with a shutdown ack
        # wait for card to shutdown
        #shutdown timmer for each card.
        #if faisl to shutdown send a timeout error saying it failed to showdown
        # eait for each card to shutdown one by one.
        for i in addresses:
            if(response & addresses[i] == shutdown ): #have it be a shutdown ack
                # for card to completely shutdown and return with finally shutdown bits
                reply = self.read(addresses[i])
                if (reply & addresses[i] == shutdown):
                    # remove address from list 


                    return
                time.sleep(3)
                print('shutdown timeout error')
                #write the card data so thats its headder is now an error header.

            else:
                print('did not respond with correct shutdown ack')

            i = i+1

        #shutdown self
         #check that all cards are empty
         # look how write codde to shutdown itself.



        ...


    def error(self, address):
        ...


# '''

# interupts would have to be card specific, figure what how they should be triggered, 
# these should done in the coding of the cards themselves but the api should have somewhat to trigger an interupt
# and this interupt can specifically be coded into the card itself.

# '''
