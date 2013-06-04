import usb.core
import usb.util
import time
from struct import *
from Queue import Queue
from threading import Thread
import csv

class TissueTesterInterface:
	started = 0;
	lastpack = []
	
	def __init__(self):
		"""Initializes the Tissue Tester Device over usb"""
		self.q = []
		self.dev = usb.core.find(idVendor=0x1cbe, idProduct=0x0003)
		if not self.dev:
			raise usb.core.USBError('USB peripherial not detected')
		self.dev.set_configuration()
		
	def fetchpacketworker(self):
		"""Internally used method. Called in a separate thread to handle and process
		USB packets intro usable data"""
		while(self.started):
			try:
				ret = self.dev.read(0x81, 64, 0, 100)
			except:
				break
			sret = ''.join([chr(x) for x in ret])
			if(not(len(ret) == 32)):
				print("ret incorrect"+str(len(ret)))
			self.q.append(unpack('=Bxxxllllllxxxx',sret[:32]))
	
	def sendcmd(self,msg,data):
		"""Sends the specified command and data to the tissue tester device. 
		The command is a single byte and data should be a signed integer value"""
		msg = pack('=Bl',msg,data)
		assert self.dev.write(1,msg,0,500) == len(msg)
	
	def start(self):
		"""Starts the Tissue Tester actuator and data collection"""
		self.started = 1
		self.sendcmd(1,0)
		self.packetfetchthread = Thread(None,self.fetchpacketworker,None)
		self.packetfetchthread.daemon = True
		self.packetfetchthread.start()
		print("launched")
	
	def stop(self):
		"""Stops the tissue tester actuator and data collection"""
		#self.packetfetchthread.stop()
		self.started = 0
		self.sendcmd(2,0)

	
	def zeroPosition(self, zero):
		"""Zeros the position sensor to the given value. If zero is 0xff, it uses the
		last sampled LVDT position to zero the device"""
		self.sendcmd(3,zero)
		
	def setController(self,type):
		"""Sets the controller type for the tissue tester.. Implemented options include:
		Position, Current", and Open Loop"""
		if type == 'Position' :
			self.sendcmd(4,0)
		elif type == 'Current' :
			self.sendcmd(4,1)
		elif type == 'Open Loop' :
			self.sendcmd(4,2)
	
	def setTrajectorySource(self,source) :
		"""Sets the trajectory source for movement of the tissue tester. Normally this will
		be from the internal lookup table on the controller. Options are Table or Streaming"""
		if source == 'Table' :
			self.sendcmd(5,0)
		elif source == 'Streaming' :
			self.sendcmd(5,1)
	
	def loadTrajectory(self, trajectory) :
		"""Provide this function with a list of 100 numbers corresponding to one period of
		motion of the indenter"""
		self.sendcmd(6,0)
		time.sleep(0.1)
		pk = []
		pk.append(pack('llllllllllllllll',trajectory[:16]))
		pk.append(pack('llllllllllllllll',trajectory[16:32]))
		pk.append(pack('llllllllllllllll',trajectory[32:48]))
		pk.append(pack('llllllllllllllll',trajectory[48:64]))
		pk.append(pack('llllllllllllllll',trajectory[64:80]))
		pk.append(pack('llllllllllllllll',trajectory[80:96]))
		pk.append(pack('llll',trajectory[96:100]))
		
		for p in pk:
			assert self.dev.write(1,p,0,100) == len(p)
			
		self.sendcmd(6,1) # tell the processor we are done
	
	def setPoint(self, setpt) :
		"""Sets the controller setpoint. Use if you are using a "Streaming" Trajectory source"""
		self.sendcmd(7,setpt)
	
	def setTrajectoryFrequency(self, freq) :
		"""Sets the Frequency with which the given trajectory is followed. Has no effect
		if setTrajectorySource is set to 'Streaming'"""
		self.sendcmd(8,freq)



