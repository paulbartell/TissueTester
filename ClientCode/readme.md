# Tissue Tester Client-side software documentation:

## Some prerequisites:
- Python 2.7 (probably will work with Python 3)
- PySide (Python QT interface)
- libusb (should be able to use the included driver file)
- pyusb (provides a python interface to libusb)

## Graphical Interface

You can run the main gui (TissueTesterExtendedUi.py) to start and top data recording and save data from the tissue tester. This is the suggested way of running the tissue tester.

An first-run simpler interface is in TissueTesterClient.py . It just allows you to set the frequency of motion, start/stop data capture, and save data files.

MainWindow.ui is the QT designer gui file for use with QT Designer
MainWindow.py is the python version of the gui file. This is a helper file for TissueTesterExtendedUi.py You can generate it from the ui file using the following command: 

'''bash
pyside-uic -x -o MainWindow.py MainWindow.ui
'''
## Tissue Tester python Interface
The python interface (in TissueTesterInterface.py) provides an easy to use python interface with the Tissue Tester for starting and stopping data capture.
This is mainly for if you are writing your own code for interacting with the device. If you are using the TissueTesterInterface, look in the code for documentation of each of the functions. Some knowledge of the code/state structure of the embedded controller code would probably be helpful.

Below is an example of initializing the TissueTesterInterface:

'''python
from TissueTesterInterface import *
tt = TissueTesterInterface()
tt.start()
# wait a bit here
tt.stop()
'''

