import usb.core
import usb.util
import time
from struct import *

dev = usb.core.find(idVendor=0x1cbe, idProduct=0x0003)


if not dev:
	raise usb.core.USBError('USB peripherial not detected')

dev.set_configuration()
#dev.is_kernel_driver_active(0)



msg = [0x01]
assert dev.write(1, msg, 0, 100) == len(msg)
time.sleep(0.001)
assert dev.write(1, msg, 0, 100) == len(msg)
while True:
	ret = dev.read(0x81, 64, 0, 100)


	print(len(ret))
#	if len(ret) == 28 :
#		upret = unpack('=BBBBLLLLLL',ret)
#	else :
#		upret = unpack('=BBBBLLLLLLBBBBLLLLLL',ret)
#	print(upret)
#	print(len(upret))
	print(ret)
	type(ret)
	time.sleep(0.001)
