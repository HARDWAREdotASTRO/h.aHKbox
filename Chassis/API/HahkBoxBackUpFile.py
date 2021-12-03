import time


from smbus import SMBus
leader = 0x00
followerResponse = 0x10 #0b0001 << 4
lRequest = 0x00 #0b0000 << 4
lResponse = 0x10#0b0001 << 4
cRequest = 0x70 #0b0111 << 4
cResponse = 0x80 #0b1000 << 4
interrupt = 0x50 #0b0101 << 4
boot = 0x60 #0b0110 << 4
error = 0x20 #0b0010 << 4
data = 0x30 #0b0011 << 4
ack = 0x40 #0b0100 << 4
shutdown = 0xe0 #0b1110 <<4 # change these bits later to fit the data sheet mroe acurately
byteRequest = 0x80 #0b1000 << 4

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
    dataFrame = bytearray([255])


    def __init__(self, address = 0x00):
        self.address = 0x00
        self.leader = SMBus(1)
  
        
        #used as default settings for the Leader. 
        #does the leader have anthing it needs to be defined as itself (maybe not, the leader is really only displaying values and request specific data 
        # from certain cards.)
        
        


    def read(self, address, byteRequest):
        
        #with self.leader as smbus(1):

            #read the first two bytes of information to see if there is data
            # first byte will contain header and card number
            # second byte will contain information about header

                # info = self.read_byte(address)
                # #if data head is for data 
                # if(info & 0xff & data == data & info << 8 == 0b0):
                #     # go next card there is no information
                #     return info

            # initalRead = self.read_word(address)
            # numByte = initalRead << 8
            # if numByte & 0x80 != 0x80:

                #do an initial read
                # if specified amount fo bytes are recived then use 
                # do nothing
  

        read = self.read_block_data(address,0, byteRequest)
        self.dataFrame[0] = read


        return read


            # will do an inital read of the first (2) bytes and if there is information 
            # it will then request to read that block ammount of data



    def write(self, address, msg):

       # with self.leader as smbus(1):
                #temp value figure out how to implement on how to write a header to cards
            #header = 0b0

                #does not return
                # logstical header and source card infor
            #self.dataFrame[0] = ... # logstical header | 0bo (master address0)

                #note may need for loop of some kind to properly intergrate through each buffer of data frames. 
            #self.dataFrame[1]


        smbus = SMBus(1)

        write = self.leader.write_block_data(address, 0, msg)
        self.dataFrame[0] = write




        ...
    
    # note to self to replace time.sleep functions with a timeout counter


    def boot(self):
        #this will intialize all addresses the Leader in the firmware should have a prelist of some addresses (maybe the location of each card)
        availableAddresses = []
        # 10 addr
        i = 0
        while i <= 10:
            self.write(boot, bin(i))
            response = self.read(bin(i))
            if response == followerResponse | bin(i):
                availableAddresses.append(bin(i) & 0xff)
                self.write(bin(i), ack)
                i = i + 1
                continue
            time.sleep(.1)
            print('Card '+ bin(i) + ' did not respond.  ')
            i = i + 1

        return availableAddresses


        #return an array of addresses
        #the poll function will be programmed in the actual firmware.

    def poll(self, me, address):
        #it wait 100ms for a response from slave
        #else it will time out and report an error
        # remove from addresses[]
        addresses = []
        addresses.append(boot())
        while(True):
            for a in addresses:
                self.write(addresses[a], lRequest | addresses[a])
                self.wait(50)
                response = self.read(addresses[a], 2)
                if(response & 0xff == 0x00):
                    self.write(addresses[a] ,addresses[a] | byteRequest)
                    information = self.read(addresses[a], response << 8)

                    # ack knowledges that information hAS BEEN recieved
                    self.write(addresses[a] ,addresses[a] | ack)
                    #need multiple read functions, 
                    #use the number of bytes to determine what read function to use.
                    # add info for after the byte request
                    return information

                if(response & 0xff != 0x00):
                    print('no information!')
                    # time.sleep(.1)
                    # print('no reponse after byte request')
                    #remove from list
                    continue
                # else:
                #     print()


                time.sleep(.1)
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
