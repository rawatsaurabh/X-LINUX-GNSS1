# web_browser.py⏎
# Copyright (c) STMicroelectronics⏎
# License: BSD-3-Clause⏎
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtWebKitWidgets import *

import serial
import time
import string
import pynmea2
import re
import sys

class MainWindow(QMainWindow):

        def __init__(self, *args, **kwargs):
                super(MainWindow,self).__init__(*args, **kwargs)

                self.setWindowTitle(sys.argv[1])
                #self.setWindowTitle("GNSS1A1-STMicroelectronics")
                self.browser = QWebView()
                #self.browser.setUrl( QUrl(sys.argv[1]) )
                #self.browser.setUrl( QUrl("https://www.google.com/maps?q=37.819722,-122.478611") )
                self.browser.setUrl( QUrl("https://www.google.com/maps?"))
                self.setCentralWidget(self.browser)
                self.show()
                while True:
                    port="/dev/ttySTM2"
                    ser=serial.Serial(port, baudrate=9600, timeout=0.5)
                    dataout = pynmea2.NMEAStreamReader()
                    newdata=ser.readline()
                    print(newdata)
                    gpsdataString=newdata.decode()
                    string_to_search="GPGLL"
                    if string_to_search in gpsdataString:
                        newmsg=pynmea2.parse(gpsdataString)
                        print(newmsg)
                        lat=newmsg.latitude
                        lng=newmsg.longitude
                        print("Latitude= "+str(lat)+"and Longitude="+str(lng))
                        self.browser.setUrl(QUrl("https://www.google.com/maps?"))
                        time.sleep(seconds)

QApplication.setAttribute(Qt.AA_ShareOpenGLContexts)
app = QApplication(sys.argv)
window = MainWindow()
