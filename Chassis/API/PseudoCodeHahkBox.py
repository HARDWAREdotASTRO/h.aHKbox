import time
import board
import busio
from smbus2 import SMbus
leader = 0b0000
followerResponse = 0b0001 << 4
lRequest = 0b0000 << 4
lResponse = 0b0001 << 4
cRequest = 0b0111 << 4
cResponse = 0b1000 << 4
interrupt = 0b0101 << 4
boot = 0b0110 << 4
error = 0b0010 << 4
data = 0b0011 << 4
ack = 0b0100 << 4
byteRequest = 0b1000 << 4

#availableAddresses = []



class Leader:
    i2c = busio.I2C(board.SCL, board.SDA)


    def __init__(self):
        
        

        ...

    def read(self, address):


        ...


    def write(self, address, msg):


        ...
    


    def boot(self, addressed):
        #this will intialize all addresses the Leader in the firmware should have a prelist of some addresses (maybe the location of each card)
        availableAddresses = []
        # 10 addr
        i = 0
        while i <= 10:
            self.write(boot,  bin(i))
            response = self.read(bin(i))
            if response == followerResponse | bin(i):
                availableAddresses.append(bin(i) & 0xff)
                self.write(bin(i), ack)
                i = i +1
                continue
            time.sleep(.1)
            print('Card '+ bin(i) + ' did not respond. If this card is plugged in please check its connection or its code that its responsding the correct information. ')
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
                self.write(addresses[a], lRequest | addresses[a] >> 4)
                self.wait(50)
                response = self.read(addresses[a])
                if(response & 0xff == bin(0)):
                    self.write(addresses[a] ,addresses[a] | byteRequest)
                    information = self.read(addresses[a])
                    return information
                if(response & 0xff != bin(0)):
                    print('no information!')
                    # time.sleep(.1)
                    # print('no reponse after byte request')
                    #remove from list
                    continue
                else:
                    print()


                time.sleep(.1)
                #remove from list
                print('did not respond')
                



            ...


        ...

    def shutdown(self):

        ...


class Follower:

    def __init__(self):

        ...

    def read(self, fromLeader):

        self._device = SMbus2.SMbus()


        ...

    def write(self, toLeader, msg):


        ...

    def boot(self, addr):
        err = False
        msg = self.read(leader)
        self.wait(50)
 
        if(msg == boot | addr):
            self.write(leader, lResponse | addr)
            
            return #returns nothing just breaks out before timeout
        time.sleep(.1)
        print("TIMEOUT ERROR: Leader device did not respond. check code or wiring")

        ...

    def shutdown(self, toLeader):


        ...
