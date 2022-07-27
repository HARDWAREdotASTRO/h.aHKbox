import smbus
from hahkBox import Leader
from newBoot import boot
from cardLibrary import wireAC
addr = 0x08
bus = smbus.SMBus(1)

leader = Leader(addr)
leader2 = boot()
address = []
address = leader.boot()
addresses = leader2.newBoot()
print(address)
print("new boot function addresses " + str(addresses))
#for i in address:
#while(True):


while(True):
	values = leader.poll(address)
	print(values)
	print(values[0])
	byte = bytearray(values[0])
	print(type(byte[0]))
	if(int((byte[0])) & 0x0f == 0x03):
		print('yes')
		wireAC(byte)
		
	
	#print(type(values))
