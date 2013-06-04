import sys
from PySide import QtGui, QtCore
from TissueTesterInterface import *
import csv

class Example(QtGui.QWidget):
	
	datafile = "TissueTesterData.csv"
	
	def __init__(self):
		super(Example, self).__init__()
		self.tt = TissueTesterInterface()
		self.initUI()
		
	def saveLogFile(self):
		header = ['startbyte','time','position','current','acceleration','cmdval','control effort']
		with open(self.datafile,'wb') as csvfile:
			cfwriter = csv.writer(csvfile, delimiter = ",")
			cfwriter.writerow(header)
			for rec in self.tt.q:
				cfwriter.writerow(rec)
				
	def clearQueue(self):
		self.tt.q = []
		
	def buttonClicked(self):
		print(self.button.text())
		if(self.button.text() == "Start"):
			self.button.setText("Stop")
			self.tt.start()
		else:
			self.button.setText("Start")
			self.tt.stop()
			
	def button2Clicked(self):
		print(self.sld.value())
		self.tt.setTrajectoryFrequency(self.sld.value())
	
	def logbuttonClicked(self):
		self.datafile = str(QtGui.QFileDialog.getSaveFileName(self, "Save file")[0])
		print("Saved Contents to a file")
		self.saveLogFile()
		
	def initUI(self):
		
		lcd = QtGui.QLCDNumber(self)
		sld = QtGui.QSlider(QtCore.Qt.Vertical, self)
		button = QtGui.QPushButton("Start", self)
		loggerbutton = QtGui.QPushButton("Save Log File",self)
		self.sld = sld
		self.loggerbutton = loggerbutton
		vbox = QtGui.QVBoxLayout()
		hbox = QtGui.QHBoxLayout()
		hbox.addWidget(lcd)
		hbox.addWidget(sld)
		vbox.addLayout(hbox)
		vbox.addWidget(button)
		vbox.addWidget(loggerbutton)
		
		self.button = button
		self.setLayout(vbox)
		sld.valueChanged.connect(lcd.display)
		sld.sliderReleased.connect(self.button2Clicked)
		button.clicked.connect(self.buttonClicked)
		loggerbutton.clicked.connect(self.logbuttonClicked)
		self.setGeometry(300, 300, 300, 200)
		self.setWindowTitle('Tissue Tester Slider Demo')
		self.show()
		

		
		
def main():
	
	app = QtGui.QApplication(sys.argv)
	ex = Example()
	sys.exit(app.exec_())


if __name__ == '__main__':
	main()