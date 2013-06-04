import sys
from PySide import QtGui, QtCore
from TissueTesterInterface import *
from MainWindow import Ui_MainWindow

class TTGui:
	def __init__(self):
		self.app = QtGui.QApplication(sys.argv)
		self.tt = TissueTesterInterface()
		self.MainWindow = QtGui.QMainWindow()
		ui = Ui_MainWindow()
		ui.setupUi(self.MainWindow)
		self.ui = ui
		self.MainWindow.show()
		self.wireGui()
			
	def wireGui(self):
		"""Set Up Handlers for our GUI"""
		ui = self.ui
		ui.frequencySlider.valueChanged.connect(ui.frequencyDisplay.display)
		ui.frequencySlider.valueChanged.connect(self.frequencyChange)
		ui.incrButton.clicked.connect(self.incrButtonHandler)
		ui.decrButton.clicked.connect(self.decrButtonHandler)
		ui.startButton.clicked.connect(self.startButtonHandler)
		ui.stopButton.clicked.connect(self.stopButtonHandler)
		ui.saveLogButton.clicked.connect(self.logButtonHandler)
		ui.clearDataButton.clicked.connect(self.clearLogData)
		ui.controlModeBox.currentIndexChanged.connect(self.controlModeChanged)
		ui.zeroPosButton.clicked.connect(self.zeroPositionHandler)
		ui.runRegressionButton.clicked.connect(self.runRegression)

		
		self.MainWindow.setWindowTitle('Tissue Tester UI')
		
	def updateDisplayData(self):
		dat = self.tt.q[len(self.tt.q)-1]
		ui = self.ui
		ui.timestampLineEdit.setText(str(dat[1]))
		ui.positionLineEdit.setText(str(dat[2]))
		ui.currentLineEdit.setText(str(dat[3]))
		ui.accelerationLineEdit.setText(str(dat[4]))
		ui.commandLineEdit.setText(str(dat[5]))
		ui.cEffortLineEdit.setText(str(dat[6]))
				
	def setObjectName():
		return 0
	
	def zeroPositionHandler(self):
		"""Sets position zero to current position"""
		self.tt.zeroPosition(0xff)
	
	def runRegression(self):
		"""Not Implemented Yet"""
		pass
		
	def incrButtonHandler(self):
		if(self.ui.frequencySlider.value() < self.ui.frequencySlider.maximum()):
			self.ui.frequencySlider.setValue(self.ui.frequencySlider.value() + 1)
	def decrButtonHandler(self):
		if(self.ui.frequencySlider.value() > self.ui.frequencySlider.minimum()):
			self.ui.frequencySlider.setValue(self.ui.frequencySlider.value() - 1)
			
	def controlModeChanged(self,index):
		self.tt.setController(self.ui.controlModeBox.itemText(index))
		print("Set Control Mode" + self.ui.controlModeBox.itemText(index))
	
	def startButtonHandler(self):
		print("Sending Start Command")
		self.tt.start()
		self.ui.startButton.setEnabled(False)
		self.ui.stopButton.setEnabled(True)
		timer = QtCore.QTimer(self.app)
		timer.timeout.connect(self.updateDisplayData)
		timer.start(100) #update at 1Hz
		self.timer = timer
			
	def stopButtonHandler(self):
		print("sending stop command")
		self.tt.stop()
		self.ui.startButton.setEnabled(True)
		self.ui.stopButton.setEnabled(False)
			
	def frequencyChange(self):
		if(not(self.ui.frequencySlider.isSliderDown())):
			print("Frequency Set to " + str(self.ui.frequencySlider.value()))
			self.tt.setTrajectoryFrequency(self.ui.frequencySlider.value())
		
	def logButtonHandler(self):
		self.datafile = str(QtGui.QFileDialog.getSaveFileName(None, "Save file")[0])
		if(self.datafile[0]):
			print("Saved Contents to a file")
			self.saveLogFile()
		
	def saveLogFile(self):
		header = ['startbyte','time','position','current','acceleration','cmdval','control effort']
		with open(self.datafile,'wb') as csvfile:
			cfwriter = csv.writer(csvfile, delimiter = ",")
			cfwriter.writerow(header)
			for rec in self.tt.q:
				cfwriter.writerow(rec)
		
	def clearLogData(self):
		self.tt.q = []
		
if __name__ == "__main__":
	mySW = TTGui()
	sys.exit(mySW.app.exec_())
