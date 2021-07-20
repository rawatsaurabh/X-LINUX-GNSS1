import serial
import time
import string
import pynmea2
import re


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
        print("Latitude= " + str(lat) + " and Longitude= " + str(lng))
